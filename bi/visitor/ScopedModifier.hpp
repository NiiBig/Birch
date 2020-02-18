/**
 * @file
 */
#pragma once

#include "bi/visitor/Modifier.hpp"
#include "bi/expression/all.hpp"
#include "bi/statement/all.hpp"
#include "bi/type/all.hpp"

#include <list>

namespace bi {
/**
 * Modifier that keeps track of the stack of scopes during traversal.
 *
 * @ingroup visitor
 */
class ScopedModifier: public Modifier {
public:
  /**
   * Constructor.
   */
  ScopedModifier();

  /**
   * Destructor.
   */
  virtual ~ScopedModifier();

  using Modifier::modify;

  virtual Package* modify(Package* o);
  virtual Expression* modify(LambdaFunction* o);
  virtual Expression* modify(Member* o);
  virtual Expression* modify(Global* o);
  virtual Statement* modify(MemberFunction* o);
  virtual Statement* modify(Function* o);
  virtual Statement* modify(MemberFiber* o);
  virtual Statement* modify(Fiber* o);
  virtual Statement* modify(BinaryOperator* o);
  virtual Statement* modify(UnaryOperator* o);
  virtual Statement* modify(AssignmentOperator* o);
  virtual Statement* modify(ConversionOperator* o);
  virtual Statement* modify(Program* o);
  virtual Statement* modify(Class* o);
  virtual Statement* modify(If* o);
  virtual Statement* modify(For* o);
  virtual Statement* modify(Parallel* o);
  virtual Statement* modify(While* o);
  virtual Statement* modify(DoWhile* o);

protected:
  /**
   * List of scopes, innermost at the back.
   */
  std::list<Scope*> scopes;

  /**
   * Are we on the right hand side of a member dereference (i.e. `b` in
   * `a.b`)?
   */
  int inMember;

  /**
   * Are we on the right hand side of a global dereference (i.e. `b` in
   * `global.b`)?
   */
  int inGlobal;

  /**
   * If in a class, a pointer to that class, otherwise `nullptr`.
   */
  Class* currentClass;

  /**
   * If in a fiber, a pointer to that fiber, otherwise `nullptr`.
   */
  Fiber* currentFiber;
};
}
