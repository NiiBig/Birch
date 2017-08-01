/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"

namespace bi {
/**
 * Object containing two other objects, denoted left and right.
 *
 * @ingroup compiler_common
 */
template<class T>
class Couple {
public:
  /**
   * Constructor.
   *
   * @param left Left.
   * @param right Right.
   */
  Couple(T* left, T* right);

  /**
   * Destructor.
   */
  virtual ~Couple() = 0;

  /**
   * Left.
   */
  T* left;

  /**
   * Right.
   */
  T* right;
};
}
