/**
 * @file
 */
#include "libbirch/Label.hpp"

libbirch::Label::Label() {
  //
}

libbirch::Label::Label(const Label& o) {
  auto& o1 = const_cast<Label&>(o);
  o1.lock.write();
  o1.memo.rehash();
  o1.lock.downgrade();
  memo.copy(o1.memo);
  o1.lock.unread();
}

libbirch::Any* libbirch::Label::get(Any* o) {
  Any* prev = nullptr;
  Any* next = o;
  bool frozen = o->isFrozen();
  while (frozen && next) {
    prev = next;
    next = memo.get(prev);
    if (next) {
      frozen = next->isFrozen();
    }
  }
  if (!next) {
	  next = prev;
	}
  if (frozen) {
    Any* cloned;
    if (next->isUnique()) {
      /* this is the last pointer to the object, recycle it */
      cloned = next->recycle_(Recycler(this));
    } else {
      /* copy it */
      cloned = next->copy_(Copier(this));
      if (!next->isFrozenUnique()) {
        memo.put(next, cloned);
      }
    }
    next = cloned;
  }
  return next;
}

libbirch::Any* libbirch::Label::pull(Any* o) {
  Any* prev = nullptr;
  Any* next = o;
  bool frozen = o->isFrozen();
  while (frozen && next) {
    prev = next;
    next = memo.get(prev);
    if (next) {
      frozen = next->isFrozen();
    }
  }
  if (!next) {
	  next = prev;
	}
  return next;
}
