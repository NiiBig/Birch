/**
 * @file
 */
#if ENABLE_LAZY_DEEP_CLONE
#include "libbirch/LazyContext.hpp"

#include "libbirch/SwapClone.hpp"
#include "libbirch/SwapContext.hpp"

libbirch::LazyAny* libbirch::LazyContext::get(LazyAny* o) {
  assert(o->isFrozen());
  LazyAny* prev = nullptr;
  LazyAny* next = o;
  bool frozen = true;
  l.write();
  do {
    prev = next;
    next = m.get(prev);
    if (next) {
      frozen = next->isFrozen();
    }
  } while (frozen && next);
  if (!next) {
	  next = prev;
	}
  if (frozen) {
    next = copy(next);
  }
  l.unwrite();
  return next;
}

libbirch::LazyAny* libbirch::LazyContext::pull(LazyAny* o) {
  assert(o->isFrozen());
  LazyAny* prev = nullptr;
  LazyAny* next = o;
  bool frozen = true;
  l.read();
  do {
    prev = next;
    next = m.get(prev);
    if (next) {
      frozen = next->isFrozen();
    }
  } while (frozen && next);
  if (!next) {
	  next = prev;
	}
  l.unread();
  return next;
}

libbirch::LazyAny* libbirch::LazyContext::copy(LazyAny* o) {
  assert(o->isFrozen());
  SwapClone swapClone(true);
  SwapContext swapContext(this);
  auto cloned = o->clone_();
  if (!o->isSingle()) {
    frozen.store(false);  // no longer frozen, as will have new entry
    m.put(o, cloned);
  }
  return cloned;
}

void libbirch::LazyContext::freeze() {
  if (!frozen.exchange(true)) {
    l.read();
    m.freeze();
    l.unread();
  }
}

#endif
