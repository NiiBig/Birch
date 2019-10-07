/**
 * @file
 */
#pragma once

#include "libbirch/Any.hpp"

namespace libbirch {
/**
 * State of a fiber.
 *
 * @ingroup libbirch
 *
 * @tparam YieldType Yield type.
 */
template<class YieldType>
class FiberState: public Any {
public:
  using class_type_ = FiberState<YieldType>;
  using yield_type_ = YieldType;

  /**
   * Constructor.
   *
   * @param npoints Number of yield points.
   */
  FiberState(Label* label, const int npoints) :
      Any(label),
      point_(0),
      npoints_(npoints) {
    //
  }

  /**
   * Deep copy constructor.
   */
  FiberState(Label* label, const FiberState& o) :
      Any(label, o),
      value_(label, o.value_),
      point_(o.point_),
      npoints_(o.npoints_) {
    //
  }

  /**
   * Destructor.
   */
  virtual ~FiberState() {
    //
  }

  /**
   * Run to next yield point.
   */
  virtual bool query() = 0;

  /**
   * Get the last yield value.
   */
  YieldType& get() {
    return value_;
  }

protected:
  /**
   * Yield value.
   */
  YieldType value_;

  /**
   * Current yield point.
   */
  int point_;

  /**
   * Number of yield points.
   */
  int npoints_;
};
}
