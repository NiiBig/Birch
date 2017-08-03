/**
 * @file
 */
#pragma once

#include "bi/lib/Object.hpp"

namespace bi {
/**
 * Fiber state.
 *
 * @ingroup library
 *
 * @tparam Type Return type.
 */
template<class Type>
class FiberState: public Object {
public:
  /**
   * Constructor.
   */
  FiberState() :
      state(0),
      nstates(0) {
    //
  }

  /**
   * Destructor.
   */
  virtual ~FiberState() {
    //
  }

  /**
   * Clone the object.
   */
  virtual FiberState<Type>* clone() = 0;

  /**
   * Run to next yield point.
   */
  virtual bool query() = 0;

  /**
   * Get the last yield value.
   */
  Type& get() {
    return value;
  }
  const Type& get() const {
    return value;
  }

protected:
  /**
   * Last yielded value.
   */
  Type value;

  /**
   * State.
   */
  int state;

  /**
   * Number of states.
   */
  int nstates;
};
}
