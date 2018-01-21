/**
 * @file
 */
#pragma once

#include "bi/visitor/Modifier.hpp"

#include <stack>

namespace bi {
/**
 * This is the first pass of the abstract syntax tree after parsing,
 * populating available types. This is analogous to having forward
 * declarations of all types as in C++.
 *
 * @ingroup visitor
 */
class Typer: public Modifier {
public:
  /**
   * Constructor.
   *
   * @param rootScope The root scope.
   */
  Typer(Scope* rootScope);

  /**
   * Destructor.
   */
  virtual ~Typer();

  using Modifier::modify;

  virtual Statement* modify(Basic* o);
  virtual Statement* modify(Class* o);

protected:
  /**
   * The root scope.
   */
  Scope* rootScope;
};
}
