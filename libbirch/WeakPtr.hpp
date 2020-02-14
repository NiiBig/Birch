/**
 * @file
 */
#pragma once

#include "libbirch/class.hpp"
#include "libbirch/Counted.hpp"

namespace libbirch {
/**
 * Weak pointer with intrusive implementation.
 *
 * @ingroup libbirch
 *
 * @tparam T Type, must derive from Counted.
 */
template<class T>
class WeakPtr: public WeakPtr<typename bi::type::super_type<T>::type> {
public:
  using value_type = T;
  using this_type = WeakPtr<T>;
  using super_type = WeakPtr<typename bi::type::super_type<value_type>::type>;

  /**
   * Constructor.
   */
  explicit WeakPtr(value_type* ptr = nullptr) :
      super_type(ptr) {
    //
  }

  /**
   * Constructor.
   */
  template<class Q, std::enable_if_t<!std::is_pointer<Q>::value && is_base_of<this_type,Q>::value,int> = 0>
  WeakPtr(const Q& o) :
      super_type(o) {
    //
  }

  /**
   * Get the raw pointer.
   */
  T* get() const {
    return static_cast<T*>(super_type::get());
  }

  /**
   * Get the raw pointer as const.
   */
  T* pull() const {
    return static_cast<T*>(super_type::pull());
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
};

/**
 * Weak pointer with intrusive implementation.
 *
 * @ingroup libbirch
 */
template<>
class WeakPtr<Counted> {
public:
  using value_type = Counted;
  using this_type = WeakPtr<Counted>;

  /**
   * Constructor.
   */
  explicit WeakPtr(value_type* ptr = nullptr) :
      ptr(ptr) {
    if (ptr) {
      ptr->incWeak();
    }
  }

  /**
   * Constructor.
   */
  template<class Q, std::enable_if_t<!std::is_pointer<Q>::value && is_base_of<this_type,Q>::value,int> = 0>
  WeakPtr(const Q& o) :
      ptr(o.get()) {
    if (ptr) {
      ptr->incWeak();
    }
  }

  /**
   * Copy constructor.
   */
  WeakPtr(const WeakPtr& o) {
    if (ptr) {
      assert(ptr->numWeak() > 0);
      ptr->incWeak();
    }
  }

  /**
   * Move constructor.
   */
  WeakPtr(WeakPtr&& o) {
    o.ptr = nullptr;
  }

  /**
   * Destructor.
   */
  ~WeakPtr() {
    release();
  }

  /**
   * Copy assignment.
   */
  WeakPtr& operator=(const WeakPtr& o) {
    if (o.ptr) {
      o.ptr->incWeak();
    }
    auto old = ptr;
    ptr = o.ptr;
    if (old) {
      old->decWeak();
    }
    return *this;
  }

  /**
   * Move assignment.
   */
  WeakPtr& operator=(WeakPtr&& o) {
    auto old = ptr;
    ptr = o.ptr;
    o.ptr = nullptr;
    if (old) {
      old->decWeak();
    }
    return *this;
  }

  /**
   * Is the pointer not null?
   *
   * This is used instead of an `operator bool()` so as not to conflict with
   * conversion operators in the referent type.
   */
  bool query() const {
    return ptr != nullptr;
  }

  /**
   * Get the raw pointer.
   */
  Counted* get() const {
    assert(!ptr || ptr->numWeak() > 0);
    return ptr;
  }

  /**
   * Get the raw pointer as const.
   */
  Counted* pull() const {
    assert(!ptr || ptr->numWeak() > 0);
    return ptr;
  }

  /**
   * Replace.
   */
  void replace(Counted* ptr) {
    assert(!ptr || ptr->numWeak() > 0);
    auto old = this->ptr;
    if (ptr) {
      ptr->incWeak();
    }
    this->ptr = ptr;
    if (old) {
      old->decWeak();
    }
  }

  /**
   * Release.
   */
  void release() {
    if (ptr) {
      ptr->decWeak();
      ptr = nullptr;
    }
  }

  /**
   * Dereference.
   */
  Counted& operator*() const {
    return *get();
  }

  /**
   * Member access.
   */
  Counted* operator->() const {
    return get();
  }

  /**
   * Equal comparison.
   */
  template<class Q>
  bool operator==(const Q& o) const {
    return get() == o.get();
  }

  /**
   * Not equal comparison.
   */
  template<class Q>
  bool operator!=(const Q& o) const {
    return get() != o.get();
  }

private:
  /**
   * Raw pointer.
   */
  Counted* ptr;
};

template<class T>
struct is_value<WeakPtr<T>> {
  static const bool value = false;
};

template<class T>
struct is_pointer<WeakPtr<T>> {
  static const bool value = true;
};

template<class T>
struct raw_type<WeakPtr<T>> {
  using type = T*;
};
}
