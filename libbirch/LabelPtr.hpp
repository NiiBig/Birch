/**
 * @file
 */
#pragma once

#include "libbirch/SharedPtr.hpp"
#include "libbirch/InitPtr.hpp"
#include "libbirch/Label.hpp"

namespace libbirch {
/**
 * Pointer to a Label object. When the referent is the current context, this
 * acts as a raw pointer, otherwise as as a shared pointer.
 *
 * @ingroup libbirch
 */
class LabelPtr {
public:
  /**
   * Default constructor.
   */
  LabelPtr() : ptr(0), cross(false) {
    //
  }

  /**
   * Value constructor.
   */
  explicit LabelPtr(Label* ptr) {
    set(ptr);
  }

  /**
   * Copy constructor.
   */
  LabelPtr(const LabelPtr& o) {
    set(o.get());
  }

  /**
   * Destructor.
   */
  ~LabelPtr() {
    release();
  }

  /**
   * Copy assignment.
   */
  LabelPtr& operator=(const LabelPtr& o) {
    replace(o.get());
    return *this;
  }

  /**
   * Get the raw pointer.
   */
  Label* get() const {
    return reinterpret_cast<Label*>(ptr);
  }

  /**
   * Replace.
   */
  void replace(Label* ptr) {
    auto old = get();
    if (ptr != old) {
      auto oldCross = isCross();
      set(ptr);
      if (old && oldCross) {
        old->decShared();
      }
    }
  }

  /**
   * Release the context.
   */
  void release() {
    if (isCross()) {
      get()->decShared();
    }
    ptr = 0;
    cross = false;
  }

  /**
   * Is this pointer crossed? A crossed pointer is to a context different to
   * that of the context in which it was created (e.g. the context of the
   * object to which it belongs).
   */
  bool isCross() const {
    return cross;
  }

  /**
   * Dereference.
   */
  Label& operator*() const {
    return *get();
  }

  /**
   * Member access.
   */
  Label* operator->() const {
    return get();
  }

  /**
   * Equal comparison.
   */
  bool operator==(const LabelPtr& o) const {
    return get() == o.get();
  }

  /**
   * Not equal comparison.
   */
  bool operator!=(const LabelPtr& o) const {
    return get() != o.get();
  }

  /**
   * Is the pointer not null?
   */
  operator bool() const {
    return ptr != 0;
  }

private:
  /**
   * Set.
   */
  void set(Label* ptr) {
    this->ptr = reinterpret_cast<intptr_t>(ptr);
    cross = ptr && ptr != currentContext;
    if (cross) {
      ptr->incShared();
    }
  }

  /**
   * Raw pointer.
   */
  intptr_t ptr:63;

  /**
   * Is this a cross pointer?
   */
  bool cross:1;
};
}
