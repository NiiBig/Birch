/**
 * @file
 */
#pragma once

#include "libbirch/SharedPtr.hpp"
#include "libbirch/InitPtr.hpp"
#include "libbirch/Memo.hpp"

namespace libbirch {
/**
 * Shared or weak pointer to a Memo, according to context. This is used by
 * LazyPtr for its memo field. It records the context of the
 * pointer: whether it is part of a member variable or not, and keeps a
 * weak or shared pointer to the Memo according to that context.
 *
 * @ingroup libbirch
 */
class ContextPtr {
public:
  /**
   * Default constructor.
   */
  ContextPtr() :
      context(currentContext) {
    //
  }

  /**
   * Value constructor.
   */
  ContextPtr(Memo* memo) :
      memo(memo == currentContext ? nullptr : memo),
      context(currentContext) {
    //
  }

  /**
   * Copy constructor.
   */
  ContextPtr(const ContextPtr& o) :
      memo(nullptr),
      context(currentContext) {
    if (o.memo) {
      if (o.memo != context) {
        memo = o.memo;
      }
    } else {
      if (o.context != context) {
        memo = o.context;
      }
    }
  }

  /**
   * Move constructor.
   */
  ContextPtr(ContextPtr&& o) :
      memo(nullptr),
      context(currentContext) {
    if (o.memo) {
      if (o.memo != context) {
        memo = std::move(o.memo);
      }
    } else {
      if (o.context != context) {
        memo = std::move(o.context);
      }
    }
  }

  /**
   * Value assignment.
   */
  ContextPtr& operator=(Memo* memo) {
    this->memo = (memo == context.get()) ? nullptr : memo;
    return *this;
  }

  /**
   * Copy assignment.
   */
  ContextPtr& operator=(const ContextPtr& o) {
    if (o.memo) {
      if (o.memo != context) {
        memo = o.memo;
      } else {
        memo = nullptr;
      }
    } else {
      if (o.context != context) {
        memo = o.context;
      } else {
        memo = nullptr;
      }
    }
    return *this;
  }

  /**
   * Move assignment.
   */
  ContextPtr& operator=(ContextPtr&& o) {
    if (o.memo) {
      if (o.memo != context) {
        memo = std::move(o.memo);
      } else {
        memo = nullptr;
      }
    } else {
      if (o.context != context) {
        memo = std::move(o.context);
      } else {
        memo = nullptr;
      }
    }
    return *this;
  }

  /**
   * Get the raw pointer.
   */
  Memo* get() const {
    return memo.get() ? memo.get() : context.get();
  }

  /**
   * Get the context.
   */
  Memo* getContext() const {
    return context.get();
  }

  /**
   * Dereference.
   */
  Memo& operator*() const {
    return *get();
  }

  /**
   * Member access.
   */
  Memo* operator->() const {
    return get();
  }

  /**
   * Equal comparison.
   */
  bool operator==(const ContextPtr& o) const {
    return get() == o.get();
  }

  /**
   * Not equal comparison.
   */
  bool operator!=(const ContextPtr& o) const {
    return get() != o.get();
  }

  /**
   * Is the pointer not null?
   */
  operator bool() const {
    return get() != nullptr;
  }

private:
  /**
   * The memo, if it is difference to context, otherwise `nullptr`.
   */
  SharedPtr<LazyMemo> memo;

  /**
   * The owning context. This is the context in which the pointer itself was
   * created. For a member variable, it is the same as the context of the
   * containing object. Because the containing object holds a WeakPtr to the
   * same object, use of an InitPtr is sufficient here.
   */
  InitPtr<LazyMemo> context;
};
}
