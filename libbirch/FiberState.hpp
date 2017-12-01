/**
 * @file
 */
#pragma once

#include "libbirch/Any.hpp"

namespace bi {
/**
 * State of a fiber.
 *
 * @ingroup libbirch
 *
 * @tparam Type Yield type.
 */
template<class Type>
class FiberState: public Any {
public:
  /**
   * Constructor.
   */
  FiberState(const int label = 0, const int nlabels = 0, const bool isBasic =
      false) :
      label(label),
      nlabels(nlabels),
      isValue(isValue) {
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

  /**
   * Is the yield type a value type?
   */
  bool yieldIsValue() const {
    return isValue;
  }

protected:
  /**
   * Last yielded value.
   */
  Type value;

  /**
   * Label to which to jump on next query.
   */
  int label;

  /**
   * Number of labels.
   */
  int nlabels;

  /**
   * Does the fiber yield a value type? When not a value type, the yield
   * result is shared between the fiber and its caller, and copy-on-write
   * mechanisms must be used accordingly.
   */
  bool isValue;
};
}
