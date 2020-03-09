/**
 * @file
 */
#pragma once

#include "bi/visitor/ContextualModifier.hpp"
#include "bi/expression/all.hpp"
#include "bi/statement/all.hpp"
#include "bi/type/all.hpp"

namespace bi {
/**
 * Apply code transformations. These are typically for syntactic sugar or
 * later convenience, and are applied before any attempt to resolve symbols.
 *
 * @ingroup visitor
 */
class Transformer: public ContextualModifier {
public:
  virtual Statement* modify(Assume* o);
  virtual Statement* modify(Class* o);
  virtual Statement* modify(Fiber* o);
  virtual Statement* modify(MemberFiber* o);

protected:
  /**
   * For start and resume functions with a void return type, check if the
   * last statement s a return; if not, add one.
   *
   * @param o The start or resume function to check and modify.
   */
  void insertReturn(Statement* o);

  /**
   * Construct the start function for a fiber.
   *
   * @param o The fiber to modify.
   */
  void createStart(Fiber* o);

  /**
   * Construct the resume functionss for a fiber.
   *
   * @param o The fiber to modify.
   */
  void createResumes(Fiber* o);
};
}
