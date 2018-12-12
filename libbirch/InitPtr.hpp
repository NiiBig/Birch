/**
 * @file
 */
#pragma once

#include "libbirch/config.hpp"

namespace bi {
template<class T> class SharedPtr;
template<class T> class WeakPtr;

/**
 * Smart pointer that does not update reference counts, but that does
 * initialize to nullptr.
 *
 * @ingroup libbirch
 *
 * @tparam T Type, must derive from Counted.
 */
template<class T>
class InitPtr {
public:
  /**
   * Constructor.
   */
  InitPtr(T* ptr = nullptr) :
      ptr(ptr) {
    //
  }

  /**
   * Destructor.
   */
  ~InitPtr() {
    #ifndef NDEBUG
    ptr = nullptr;
    #endif
  }

  /**
   * Get the raw pointer.
   */
  T* get() const {
    return ptr;
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
  bool operator==(const InitPtr<T>& o) const {
    return ptr == o.ptr;
  }

  /**
   * Not equal comparison.
   */
  bool operator!=(const InitPtr<T>& o) const {
    return ptr != o.ptr;
  }

  /**
   * Is the pointer not null?
   */
  operator bool() const {
    return ptr != nullptr;
  }

  /**
   * Cast to raw pointer.
   */
  operator T*() const {
    return ptr;
  }

private:
  /**
   * Raw pointer.
   */
  T* ptr;
};
}
