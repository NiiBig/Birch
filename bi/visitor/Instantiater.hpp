/**
 * @file
 */
#pragma once

#include "bi/visitor/Cloner.hpp"
#include "bi/type/TypeIterator.hpp"

namespace bi {
/**
 * Instantiates a class with generic type parameters by cloning, replacing
 * the base class of those type parameters with more-specific types, provided
 * as the generic type arguments.
 *
 * @ingroup birch_visitor
 */
class Instantiater : public Cloner {
public:
  /**
   * Constructor.
   *
   * @param Generic type arguments.
   */
  Instantiater(Type* typeArgs);

  virtual Expression* clone(const Generic* o);

protected:
  /**
   * Iterator over generic type arguments.
   */
  TypeIterator iter;
};
}
