/**
 * @file
 */
#pragma once

#include "bi/exception/CompilerException.hpp"
#include "bi/common/Argumented.hpp"
#include "bi/common/Parameterised.hpp"

#include <list>

namespace bi {
/**
 * Invalid function call.
 *
 * @ingroup compiler_exception
 */
struct CallException: public CompilerException {
  /**
   * Constructor.
   *
   * @param o The invalid call.
   * @param available The available overloads.
   */
  CallException(Argumented* o,
      const std::list<Parameterised*>& available = std::list<Parameterised*>());
};
}
