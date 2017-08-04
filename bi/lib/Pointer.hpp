/**
 * @file
 */
#pragma once

namespace bi {
/**
 * Smart pointer for global and fiber-local objects, with copy-on-write
 * semantics for te latter.
 *
 * @ingroup library
 *
 * @tparam T Type.
 */
template<class T>
class Pointer {
  template<class U> friend class Pointer;
  friend class Object;
public:
  /**
   * Constructor.
   */
  Pointer(T* raw = nullptr);

  /**
   * Generic constructor.
   */
  template<class U>
  Pointer(U* raw = nullptr);

  /**
   * Copy constructor.
   */
  Pointer(const Pointer<T>& o) = default;

  /**
   * Generic copy constructor.
   */
  template<class U>
  Pointer(const Pointer<U>& o);

  /**
   * Assignment operator.
   */
  Pointer<T>& operator=(const Pointer<T>& o) = default;

  /**
   * Generic assignment operator.
   */
  template<class U, typename = std::enable_if_t<
      std::is_base_of<T,U>::value && !std::is_same<T,U>::value>>
  Pointer<T>& operator=(const Pointer<U>& o) {
    ptr = o.ptr;
    index = o.index;
    return *this;
  }

  /**
   * Raw pointer assignment operator.
   */
  Pointer<T>& operator=(T* raw);

  /**
   * Null pointer assignment operator.
   */
  Pointer<T>& operator=(const std::nullptr_t&) {
    ptr = nullptr;
    index = -1;
    return *this;
  }

  /**
   * Generic value assignment operator.
   */
  template<class U>
  Pointer<T>& operator=(const U& o);

  /**
   * User-defined conversions. This allows pointers to be passed as arguments
   * to functions with value type parameters, where the type of the object
   * pointed to has a conversion to the value type.
   *
   * @seealso has_conversion
   */
  template<class U, typename = std::enable_if_t<has_conversion<T,U>::value>>
  operator U() {
    /* conversion operators in generated code are marked explicit, so the
     * cast is necessary here */
    return static_cast<U>(*get());
  }

  /**
   * Get the raw pointer.
   */
  T* get();
  T* const get() const;

  /**
   * Dereference.
   */
  T& operator*() {
    return *get();
  }
  const T& operator*() const {
    return *get();
  }

  /**
   * Member access.
   */
  T* operator->() {
    return get();
  }
  T* const operator->() const {
    return get();
  }

  /**
   * Call operator.
   */
  template<class ... Args>
  auto operator()(Args ... args) const {
    return (*ptr)(args...);
  }

  /*
   * Equality operators.
   */
  bool operator==(const Pointer<T>& o) const {
    return get() == o;
  }
  bool operator!=(const Pointer<T>& o) const {
    return !(*this == o);
  }

private:
  /**
   * Constructor for pointer_from_this() in Object.
   */
  Pointer(T* raw, intptr_t index);

  /**
   * For a global pointer, the raw address.
   */
  T* ptr;

  /**
   * For a fiber-local pointer, the index of the heap allocation,
   * otherwise -1.
   */
  intptr_t index;

  /// @todo Might there be an implementation that allows both cases to be
  /// packed into the same 64-bit value?
};
}

#include "bi/lib/global.hpp"
#include "bi/lib/Fiber.hpp"

template<class T>
bi::Pointer<T>::Pointer(T* raw) {
  if (currentFiber && raw) {
    ptr = nullptr;
    index = currentFiber->put(raw);
    raw->setIndex(index);
  } else {
    ptr = raw;
    index = -1;
  }
}

template<class T>
template<class U>
bi::Pointer<T>::Pointer(U* raw) {
  if (currentFiber && raw) {
    ptr = nullptr;
    index = currentFiber->put(raw);
    raw->setIndex(index);
  } else {
    ptr = raw;
    index = -1;
  }
}

template<class T>
template<class U>
bi::Pointer<T>::Pointer(const Pointer<U>& o) :
    ptr(o.ptr),
    index(o.index) {
  //
}

template<class T>
bi::Pointer<T>& bi::Pointer<T>::operator=(T* raw) {
  if (currentFiber && raw) {
    ptr = nullptr;
    index = currentFiber->put(raw);
    raw->setIndex(index);
  } else {
    ptr = raw;
    index = -1;
  }
  return *this;
}

template<class T>
template<class U>
bi::Pointer<T>& bi::Pointer<T>::operator=(const U& o) {
  *this = o;
  return *this;
}

template<class T>
T* bi::Pointer<T>::get() {
  T* raw;
  if (index >= 0) {
    assert(currentFiber);
    raw = static_cast<T*>(currentFiber->get(index));
    if (raw->isShared()) {
      /* shared and writeable, copy now (copy-on-write) */
      raw->disuse();
      raw = raw->clone();
      currentFiber->set(index, raw);
    }
  } else {
    raw = ptr;
  }
  return raw;
}

template<class T>
T* const bi::Pointer<T>::get() const {
  T* raw;
  if (index >= 0) {
    assert(currentFiber);
    raw = static_cast<T*>(currentFiber->get(index));
  } else {
    raw = ptr;
  }
  return raw;
}

template<class T>
bi::Pointer<T>::Pointer(T* raw, intptr_t index) :
    ptr(index <= 0 ? raw : nullptr),
    index(index) {
  assert(index <= 0 || currentFiber->get(index) == raw);
}
