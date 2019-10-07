/**
 * @file
 */
#pragma once
#if ENABLE_LAZY_DEEP_CLONE

#include "libbirch/Counted.hpp"
#include "libbirch/LazyAny.hpp"
#include "libbirch/LazyMemo.hpp"
#include "libbirch/SharedPtr.hpp"
#include "libbirch/ExclusiveLock.hpp"

namespace libbirch {
/**
 * Label for bookkeeping lazy deep clones.
 *
 * @ingroup libbirch
 */
class LazyLabel: public Counted {
  friend class List;
public:
  using class_type_ = LazyLabel;

  /**
   * Constructor for root node.
   */
  LazyLabel();

  /**
   * Constructor for non-root node.
   *
   * @param parent Parent.
   */
  LazyLabel(LazyLabel* parent);

  /**
   * Destructor.
   */
  virtual ~LazyLabel();

  /**
   * Fork to create a new child label.
   *
   * @return The child label.
   */
  LazyLabel* fork();

  /**
   * Map an object that may not yet have been cloned, cloning it if
   * necessary.
   */
  LazyAny* get(LazyAny* o);

  /**
   * Map an object that may not yet have been cloned, without cloning it.
   * This is used as an optimization for read-only access.
   */
  LazyAny* pull(LazyAny* o);

  /**
   * Shallow copy.
   */
  LazyAny* copy(LazyAny* o);

  /**
   * Freeze all values in the memo.
   */
  void freeze();

  /**
   * Thaw the memo.
   */
  void thaw();

  virtual const char* name_() const {
    return "LazyLabel";
  }

private:
  /**
   * Memo that maps source objects to clones.
   */
  LazyMemo m;

  /**
   * Lock.
   */
  ExclusiveLock l;

  /**
   * Is this frozen? Unlike regular objects, a memo can still have new entries
   * written after it is frozen, but this flags it as unfrozen again.
   */
  bool frozen;
};
}

inline libbirch::LazyLabel::LazyLabel() :
    frozen(false) {
  //
}

inline libbirch::LazyLabel::LazyLabel(LazyLabel* parent) :
    frozen(parent->frozen) {
  assert(parent);
  parent->l.set();
  m.copy(parent->m);
  parent->l.unset();
}

inline libbirch::LazyLabel::~LazyLabel() {
  //
}

inline libbirch::LazyLabel* libbirch::LazyLabel::fork() {
  return new LazyLabel(this);
}

#endif
