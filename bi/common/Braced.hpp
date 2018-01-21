/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"

namespace bi {
/**
 * Object with braces.
 *
 * @ingroup common
 */
class Braced {
public:
  /**
   * Constructor.
   *
   * @param braces Body.
   */
  Braced(Statement* braces);

  /**
   * Destructor.
   */
  virtual ~Braced() = 0;

  /**
   * Body.
   */
  Statement* braces;
};
}
