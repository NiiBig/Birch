/**
 * @file
 */
#pragma once

#include "libbirch/FiberState.hpp"

namespace bi {
/**
 * Fiber.
 *
 * @ingroup libbirch
 *
 * @tparam Type Yield type.
 */
template<class Type>
class Fiber {
public:
  /**
   * Constructor.
   */
  Fiber(const std::shared_ptr<FiberState<Type>>& state = nullptr,
      const bool isClosed = false);

  /**
   * Copy constructor.
   */
  Fiber(const Fiber<Type>& o);

  /**
   * Move constructor.
   */
  Fiber(Fiber<Type> && o) = default;

  /**
   * Copy assignment.
   */
  Fiber<Type>& operator=(const Fiber<Type>& o);

  /**
   * Move assignment.
   */
  Fiber<Type>& operator=(Fiber<Type> && o) = default;

  /**
   * Run to next yield point.
   *
   * @return Was a value yielded?
   */
  bool query();

  /**
   * Get the last yield value.
   *
   * @internal Returns by value to ensure that pointers, from the fiber's
   * world, are mapped to the caller's world.
   */
  const Type get() const;

private:
  /**
   * Mark a fiber as dirty, when it is copied.
   *
   * @internal This must be const as it is applied, via a const_cast, to the
   * argumet of the copy constructor and assignment operator.
   */
  void dirty() const;

  /**
   * Isolate a fiber, when it is queried while dirty. A new world is created
   * for the fiber, with its parent the existing world.
   */
  void isolate();

  /**
   * Fiber state.
   */
  std::shared_ptr<FiberState<Type>> state;

  /**
   * Fiber world.
   */
  std::shared_ptr<World> world;

  /**
   * Is the fiber closed?
   */
  bool isClosed;

  /**
   * Is the fiber dirty?
   */
  bool isDirty;
};
}

#include "libbirch/World.hpp"

template<class Type>
bi::Fiber<Type>::Fiber(const std::shared_ptr<FiberState<Type>>& state,
    const bool isClosed) :
    state(state),
    world(isClosed ? std::make_shared<World>() : fiberWorld),
    isClosed(isClosed),
    isDirty(false) {
  //
}

template<class Type>
bi::Fiber<Type>::Fiber(const Fiber<Type>& o) :
    state(o.state),
    world(o.world),
    isClosed(o.isClosed),
    isDirty(o.isDirty) {
  if (isClosed) {
    dirty();
    o.dirty();
  }
}

template<class Type>
bi::Fiber<Type>& bi::Fiber<Type>::operator=(const Fiber<Type>& o) {
  state = o.state;
  world = o.world;
  isClosed = o.isClosed;
  isDirty = o.isDirty;
  if (isClosed) {
    dirty();
    o.dirty();
  }
  return *this;
}

template<class Type>
bool bi::Fiber<Type>::query() {
  bool result = false;
  if (state) {
    if (isDirty) {
      isolate();
    }
    auto prevWorld = fiberWorld;
    fiberWorld = world;
    result = state->query();
    fiberWorld = prevWorld;
  }
  return result;
}

template<class Type>
const Type bi::Fiber<Type>::get() const {
  assert(state);
  return state->get();
}

template<class Type>
void bi::Fiber<Type>::dirty() const {
  const_cast<Fiber<Type>*>(this)->isDirty = true;
}

template<class Type>
void bi::Fiber<Type>::isolate() {
  assert(state);
  assert(world);
  assert(isDirty);
  assert(isClosed);

  state = state->clone();
  world = std::make_shared<World>(world);
  isDirty = false;
}
