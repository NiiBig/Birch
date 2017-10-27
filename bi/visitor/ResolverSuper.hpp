/**
 * @file
 */
#pragma once

#include "bi/visitor/Resolver.hpp"

namespace bi {
/**
 * This is the second pass of the abstract syntax tree after parsing,
 * establishing super type and conversion type relationships, and resolving
 * generic type parameters.
 *
 * @ingroup compiler_visitor
 */
class ResolverSuper: public Resolver {
public:
  /**
   * Constructor.
   */
  ResolverSuper();

  /**
   * Destructor.
   */
  virtual ~ResolverSuper();

  using Resolver::modify;

  virtual Expression* modify(Generic* o);
  virtual Statement* modify(Basic* o);
  virtual Statement* modify(Class* o);
  virtual Statement* modify(Alias* o);
  virtual Statement* modify(GlobalVariable* o);
  virtual Statement* modify(Function* o);
  virtual Statement* modify(Fiber* o);
  virtual Statement* modify(Program* o);
  virtual Statement* modify(BinaryOperator* o);
  virtual Statement* modify(UnaryOperator* o);
  virtual Statement* modify(MemberVariable* o);
  virtual Statement* modify(MemberFunction* o);
  virtual Statement* modify(MemberFiber* o);
  virtual Statement* modify(ConversionOperator* o);
  virtual Statement* modify(AssignmentOperator* o);
};
}
