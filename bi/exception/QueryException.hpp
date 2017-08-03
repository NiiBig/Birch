/**
 * @file
 */
#pragma once

#include "bi/exception/CompilerException.hpp"
#include "bi/expression/Query.hpp"

namespace bi {
/**
 * Invalid use of "?" query operator.
 *
 * @ingroup compiler_exception
 */
struct QueryException: public CompilerException {
  /**
   * Constructor.
   */
  QueryException(const Query* o);
};
}
