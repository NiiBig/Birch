/**
 * @file
 */
#pragma once

#include "bi/exception/CompilerException.hpp"
#include "bi/statement/Assignment.hpp"

namespace bi {
/**
 * Left hand side of assignment operator is not assignable.
 *
 * @ingroup exception
 */
struct NotAssignableException: public CompilerException {
  /**
   * Constructor.
   */
  NotAssignableException(const Assignment* o);
};
}
