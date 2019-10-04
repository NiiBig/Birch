/**
 * @file
 */
#pragma once
#if ENABLE_LAZY_DEEP_CLONE

#include "libbirch/LazyAny.hpp"
#include "libbirch/LazyContext.hpp"
#include "libbirch/Nil.hpp"
#include "libbirch/ContextPtr.hpp"

namespace libbirch {
/**
 * Wraps another pointer type to apply lazy deep clone semantics.
 *
 * @ingroup libbirch
 *
 * @tparam P Pointer type.
 */
template<class P>
class LazyPtr {
  template<class U> friend class LazyPtr;
public:
  using T = typename P::value_type;
  template<class U> using cast_type = LazyPtr<typename P::template cast_type<U>>;

  /**
   * Constructor.
   */
  LazyPtr(const Nil& = nil) {
    //
  }

  /**
   * Constructor.
   */
  explicit LazyPtr(T* object) :
      object(object) {
    //
  }

  /**
   * Constructor.
   */
  LazyPtr(const P& object) :
      object(object) {
    //
  }

	/**
	 * Copy constructor.
	 */
  LazyPtr(const LazyPtr<P>& o) :
      to(o.to) {
    object.replace(o.get());
  }

  /**
   * Generic copy constructor.
   */
  template<class Q, typename = std::enable_if_t<std::is_base_of<T,
      typename Q::value_type>::value>>
  LazyPtr(const LazyPtr<Q>& o) :
      to(o.to) {
    object.replace(o.get());
  }

  /**
   * Move constructor.
   */
  LazyPtr(LazyPtr<P> && o) = default;

  /**
   * Copy assignment.
   */
  LazyPtr<P>& operator=(const LazyPtr<P>& o) {
    /* risk of invalidating `o` here, ensure assign to `to` first */
    to = o.to;
    object.replace(o.get());
    return *this;
  }

  /**
   * Generic copy assignment.
   */
  template<class Q, typename = std::enable_if_t<std::is_base_of<T,
      typename Q::value_type>::value>>
  LazyPtr<P>& operator=(const LazyPtr<Q>& o) {
    /* risk of invalidating `o` here, ensure assign to `to` first */
    to = o.to;
    object.replace(o.get());
    return *this;
  }

  /**
   * Move assignment.
   */
  LazyPtr<P>& operator=(LazyPtr<P> && o) {
    /* risk of invalidating `o` here, ensure assign to `to` first */
    to = std::move(o.to);
    object = std::move(o.object);
    return *this;
  }

  /**
   * Generic move assignment.
   */
  template<class Q, typename = std::enable_if_t<std::is_base_of<T,
      typename Q::value_type>::value>>
  LazyPtr<P>& operator=(LazyPtr<Q> && o) {
    /* risk of invalidating `o` here, ensure assign to `to` first */
    to = std::move(o.to);
    object = std::move(o.object);
    return *this;
  }

  /**
   * Value assignment.
   */
  LazyPtr<P>& operator=(const P& o) {
    object = o;
    to.release();
    return *this;
  }

  /**
   * Nil assignment.
   */
  LazyPtr<P>& operator=(const Nil&) {
    object.release();
    to.release();
    return *this;
  }

  /**
   * Nullptr assignment.
   */
  LazyPtr<P>& operator=(const std::nullptr_t&) {
    object.release();
    to.release();
    return *this;
  }

  /**
   * Value assignment.
   */
  template<class U>
  LazyPtr<P>& operator=(const U& o) {
    *get() = o;
    return *this;
  }

  /**
   * Value conversion.
   */
  template<class U, typename = std::enable_if_t<std::is_convertible<T,U>::value>>
  operator U() const {
    return static_cast<U>(*get());
  }

  /**
   * Is the pointer not null?
   */
  bool query() const {
    return static_cast<bool>(object);
  }

  /**
   * Get the raw pointer, with lazy cloning.
   */
  T* get() {
    auto raw = object.get();
    if (raw && raw->isFrozen()) {
      raw = static_cast<T*>(to->get(raw));
      object.replace(raw);
    }
    return raw;
  }

  /**
   * Get the raw pointer, with lazy cloning.
   */
  T* get() const {
    return const_cast<LazyPtr<P>*>(this)->get();
  }

  /**
   * Get the raw pointer for read-only use, without cloning.
   */
  T* pull() {
    auto raw = object.get();
    if (raw && raw->isFrozen()) {
      raw = static_cast<T*>(to->pull(raw));
      object.replace(raw);
    }
    return raw;
  }

  /**
   * Get the raw pointer for read-only use, without cloning.
   */
  T* pull() const {
    return const_cast<LazyPtr<P>*>(this)->pull();
  }

  /**
   * Deep clone.
   */
  LazyPtr<P> clone() const {
    assert(object);
    pull();
    startFreeze();
    return LazyPtr<P>(object, to->fork());
  }

  /**
   * Start a freeze operation. This is just like freeze(), but manages thread
   * safety on entry and exit.
   */
  void startFreeze() {
    if (object) {
      freezeLock.enter();
      freeze();
      freezeLock.exit();
    }
  }

  /**
   * Start a freeze operation. This is just like freeze(), but manages thread
   * safety on entry and exit.
   */
  void startFreeze() const {
    return const_cast<LazyPtr<P>*>(this)->startFreeze();
  }

  /**
   * Freeze.
   */
  void freeze() {
    if (object) {
      object->freeze();
      to->freeze();
    }
  }

  /**
   * Freeze.
   */
  void freeze() const {
    return const_cast<LazyPtr<P>*>(this)->freeze();
  }

  /**
   * Thaw.
   */
  void thaw(LazyContext* context) {
    if (isCross()) {
      startFinish();
      startFreeze();
    }
    if (object) {
      to.replace(context);
    }
  }

  /**
   * Thaw.
   */
  void thaw(LazyContext* context) const {
    return const_cast<LazyPtr<P>*>(this)->thaw(context);
  }

  /**
   * Start a finish operation. This is just like finish(), but manages thread
   * safety on entry and exit.
   */
  void startFinish() {
    if (object) {
      finishLock.enter();
      finish();
      finishLock.exit();
    }
  }

  /**
   * Start a freeze operation. This is just like finish(), but manages thread
   * safety on entry and exit.
   */
  void startFinish() const {
    return const_cast<LazyPtr<P>*>(this)->startFinish();
  }

  /**
   * Finish.
   */
  void finish() {
    if (object) {
      get();
      object->finish();
    }
  }

  /**
   * Finish.
   */
  void finish() const {
    return const_cast<LazyPtr<P>*>(this)->finish();
  }

  /**
   * Does this pointer result from a cross copy?
   */
  bool isCross() const {
    return to.isCross();
  }

  /**
   * Dereference.
   */
  T& operator*() const {
    return *get();
  }

  /**
   * Member access.
   */
  T* operator->() const {
    return get();
  }

  /**
   * Equal comparison.
   */
  template<class U>
  bool operator==(const LazyPtr<U>& o) const {
    return get() == o.get();
  }

  /**
   * Not equal comparison.
   */
  template<class U>
  bool operator!=(const LazyPtr<U>& o) const {
    return get() != o.get();
  }

  /**
   * Dynamic cast. Returns `nullptr` if unsuccessful.
   */
  template<class U>
  auto dynamic_pointer_cast() const {
    return cast_type<U>(dynamic_cast<U*>(object.get()), to.get());
  }

  /**
   * Static cast. Undefined if unsuccessful.
   */
  template<class U>
  auto static_pointer_cast() const {
    return cast_type<U>(static_cast<U*>(object.get()), to.get());
  }

protected:
  /**
   * Constructor (used by casts).
   */
  LazyPtr(T* object, LazyContext* to) {
    this->to.replace(to);
    this->object.replace(object);
  }

  /**
   * Constructor (used by clone).
   */
  LazyPtr(const P& object, LazyContext* to) :
      to(to),
      object(object) {
    //
  }

  /**
   * Context to which to map the object.
   */
  ContextPtr to;

  /**
   * Object.
   */
  P object;
};
}

#endif
