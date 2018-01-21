/**
 * @file
 */
#pragma once

#include "bi/exception/CompilerException.hpp"
#include "bi/statement/Statement.hpp"

namespace bi {
/**
 * Incorrect yield type for fiber declaration.
 *
 * @ingroup exception
 */
struct FiberTypeException: public CompilerException {
  /**
   * Constructor.
   *
   * @param o Fiber declaration.
   */
  FiberTypeException(const Statement* o);
};
}
