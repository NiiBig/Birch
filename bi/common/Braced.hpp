/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"
#include "bi/primitive/unique_ptr.hpp"

namespace bi {
/**
 * Object with braces.
 *
 * @ingroup compiler_common
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
  unique_ptr<Statement> braces;
};
}
