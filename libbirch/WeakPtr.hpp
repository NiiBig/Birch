/**
 * @file
 */
#pragma once

#include "libbirch/Any.hpp"
#include "libbirch/type.hpp"

namespace libbirch {
/**
 * Weak pointer with intrusive implementation.
 *
 * @ingroup libbirch
 *
 * @tparam T Type, must derive from Any.
 */
template<class T>
class WeakPtr {
  template<class U> friend class SharedPtr;
  template<class U> friend class WeakPtr;
  template<class U> friend class InitPtr;
public:
  using value_type = T;

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
   * Copy constructor.
   */
  WeakPtr(const WeakPtr& o) :
      ptr(o.ptr) {
    if (ptr) {
      ptr->incWeak();
    }
  }

  /**
   * Generic copy constructor.
   */
  template<class Q, class U = typename Q::value_type,
      std::enable_if_t<std::is_base_of<T,U>::value,int> = 0>
  WeakPtr(const Q& o) :
      ptr(o.ptr) {
    if (ptr) {
      ptr->incWeak();
    }
  }

  /**
   * Move constructor.
   */
  WeakPtr(WeakPtr&& o) :
      ptr(o.ptr) {
    o.ptr = nullptr;
  }

  /**
   * Generic move constructor.
   */
  template<class U, std::enable_if_t<std::is_base_of<T,U>::value,int> = 0>
  WeakPtr(WeakPtr<U>&& o) :
      ptr(o.ptr) {
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
    auto old = ptr;
    ptr = o.ptr;
    if (ptr) {
      ptr->incWeak();
    }
    if (old) {
      old->decWeak();
    }
    return *this;
  }

  /**
   * Generic copy assignment.
   */
  template<class Q, class U = typename Q::value_type,
      std::enable_if_t<std::is_base_of<T,U>::value,int> = 0>
  WeakPtr& operator=(const Q& o) {
    auto old = ptr;
    ptr = o.ptr;
    if (ptr) {
      ptr->incWeak();
    }
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
   * Generic move assignment.
   */
  template<class U, std::enable_if_t<std::is_base_of<T,U>::value,int> = 0>
  WeakPtr& operator=(WeakPtr<U>&& o) {
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
  T* get() const {
    assert(!ptr || ptr->numWeak() > 0);
    return ptr;
  }

  /**
   * Get the raw pointer as const.
   */
  T* pull() const {
    assert(!ptr || ptr->numWeak() > 0);
    return ptr;
  }

  /**
   * Replace.
   */
  void replace(T* ptr) {
    auto old = this->ptr;
    this->ptr = ptr;
    if (ptr) {
      ptr->incWeak();
    }
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
  T& operator*() const {
    return *get();
  }

  /**
   * Member access.
   */
  T* operator->() const {
    return get();
  }

private:
  /**
   * Raw pointer.
   */
  T* ptr;
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
struct raw<WeakPtr<T>> {
  using type = T*;
};
}
