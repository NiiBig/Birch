/**
 * @file
 */
#pragma once

#include "bi/common/Scope.hpp"

namespace bi {
class Scope;

/**
 * Statement with a scope.
 *
 * @ingroup compiler_common
 */
class Scoped {
public:
  /**
   * Constructor.
   */
  Scoped();

  /**
   * Destructor.
   */
  virtual ~Scoped() = 0;

  /**
   * Scope.
   */
  Scope* scope;
};
}
