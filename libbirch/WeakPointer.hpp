/**
 * @file
 */
#pragma once

#include "libbirch/SharedPointer.hpp"
#include "libbirch/Optional.hpp"

namespace bi {
/**
 * Shared pointer with copy-on-write semantics.
 *
 * @ingroup libbirch
 *
 * @tparam T Type.
 */
template<class T>
class WeakPointer: public WeakPointer<typename super_type<T>::type> {
  template<class U> friend class SharedPointer;
  template<class U> friend class WeakPointer;
public:
  using value_type = T;
  using this_type = WeakPointer<T>;
  using super_type = WeakPointer<typename super_type<T>::type>;
  using root_type = typename super_type::root_type;

  /**
   * Default constructor.
   */
  WeakPointer(const std::nullptr_t& o = nullptr) {
    //
  }

  /**
   * Generic copy constructor.
   */
  template<class U>
  WeakPointer(const WeakPointer<U>& o) :
      super_type(o) {
    //
  }

  /**
   * Generic copy constructor from shared pointer.
   */
  template<class U>
  WeakPointer(const SharedPointer<U>& o) :
      super_type(o) {
    //
  }

  /**
   * Generic copy constructor from optional shared pointer.
   */
  template<class U>
  WeakPointer(const Optional<SharedPointer<U>>& o) :
      super_type(o) {
    //
  }
};

template<>
class WeakPointer<Any> {
  template<class U> friend class SharedPointer;
  template<class U> friend class WeakPointer;
public:
  using value_type = Any;
  using this_type = WeakPointer<value_type>;
  using root_type = this_type;

  WeakPointer(const std::nullptr_t& o = nullptr) {
    //
  }

  template<class U>
  WeakPointer(const WeakPointer<U>& o) :
      ptr(o.ptr) {
    //
  }

  template<class U>
  WeakPointer(const SharedPointer<U>& o) :
      ptr(o.ptr) {
    //
  }

  template<class U>
  WeakPointer(const Optional<SharedPointer<U>>& o) :
      WeakPointer(o.query() ? o.get() : nullptr) {
    //
  }

  WeakPointer<Any>& operator=(const WeakPointer<Any>& o) {
    assert(!o.ptr.lock() || o.ptr.lock()->getWorld() == fiberWorld);
    ptr = o.ptr;
    return *this;
  }

protected:
  /**
   * Weak pointer to the object.
   */
  std::weak_ptr<Any> ptr;
};
}
