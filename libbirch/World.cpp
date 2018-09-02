/**
 * @file
 */
#include "libbirch/World.hpp"

#include "libbirch/Any.hpp"
#include "libbirch/Enter.hpp"
#include "libbirch/Clone.hpp"

bi::World::World() :
    launchSource(fiberWorld),
    launchDepth(fiberWorld->launchDepth + 1) {
  //
}

bi::World::World(int) :
    launchDepth(0) {
  //
}

bi::World::World(const SharedPtr<World>& cloneSource) :
    cloneSource(cloneSource),
    launchSource(fiberWorld),
    launchDepth(cloneSource->launchDepth) {
  //
}

bi::World::~World() {
  //
}

void bi::World::destroy() {
  size = sizeof(*this);
  this->~World();
}

bool bi::World::hasCloneAncestor(World* world) const {
  return this == world
      || (cloneSource && cloneSource->hasCloneAncestor(world));
}

bool bi::World::hasLaunchAncestor(World* world) const {
  return this == world
      || (launchSource && launchSource->hasLaunchAncestor(world));
}

int bi::World::depth() const {
  return launchDepth;
}

bi::Any* bi::World::get(Any* o, World* world) {
  assert(o);
  int d = depth() - world->depth();
  assert(d >= 0);
  auto dst = this;
  for (int i = 0; i < d; ++i) {
    dst = dst->launchSource;
    assert(dst);
  }
  assert(dst->hasCloneAncestor(world));
  return dst->pull(o, world);
}

bi::Any* bi::World::getNoCopy(Any* o, World* world) {
  assert(o);
  int d = depth() - world->depth();
  assert(d >= 0);
  auto dst = this;
  for (int i = 0; i < d; ++i) {
    dst = dst->launchSource;
    assert(dst);
  }
  assert(dst->hasCloneAncestor(world));
  return dst->pullNoCopy(o, world);
}

bi::Any* bi::World::pull(Any* o, World* world) {
  assert(o && hasCloneAncestor(world));

  /* map */
  Any* mapped = o;
  size_t i;
  if (this != world) {
    map.startWrite();
    i = map.hash(o);
    mapped = map.get(o, i);
    if (!mapped) {
      mapped = map.put(o, cloneSource->pullNoCopy(o, world), i);
    }
    map.finishWrite();
  }

  /* copy */
  Any* result = mapped;
  if (this != mapped->getWorld()) {
    map.startWrite();
    Enter enter(this);
    Clone clone;
    SharedPtr<Any> copied = mapped->clone();
    i = map.hash(mapped);
    result = map.set(mapped, copied.get(), i);
    map.finishWrite();
  }
  return result;
}

bi::Any* bi::World::pullNoCopy(Any* o, World* world) {
  assert(o && hasCloneAncestor(world));

  /* map */
  Any* mapped = o;
  size_t i;
  if (this != world) {
    map.startWrite();
    i = map.hash(o);
    mapped = map.get(o, i);
    if (!mapped) {
      mapped = map.put(o, cloneSource->pullNoCopy(o, world), i);
    }
    map.finishWrite();
  }

  /* check for previous copy */
  Any* result = mapped;
  if (this != mapped->getWorld() && o != mapped && !map.empty()) {
    map.startRead();
    i = map.hash(mapped);
    mapped = map.get(mapped, i);
    map.finishRead();
    if (mapped) {
      result = mapped;
    }
  }
  return result;
}
