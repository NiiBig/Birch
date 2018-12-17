/**
 * @file
 */
#include "libbirch/Any.hpp"

#include "libbirch/memory.hpp"

bi::Any::Any() :
    context(top_context()->forwardGet()) {
  //
}

bi::Any::Any(const Any& o) :
    Counted(o),
    context(top_context()) {
  //
}

bi::Any::~Any() {
  //
}

bi::Memo* bi::Any::getContext() {
  return context.get();
}

void bi::Any::recordClone(Any* o) {
  #if USE_LAZY_DEEP_CLONE_FORWARD_CLEAN
  clones.put(o);
  #endif
}
