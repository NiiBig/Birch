/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Typed.hpp"
#include "bi/common/TypeArgumented.hpp"
#include "bi/common/Scope.hpp"

namespace bi {
/**
 * Name in the context of an expression, referring to a variable, function,
 * fiber, or operator.
 *
 * @ingroup expression
 */
class NamedExpression:
    public Expression,
    public Named,
    public Typed,
    public TypeArgumented {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param typeArgs Type arguments.
   * @param loc Location.
   */
  NamedExpression(Name* name, Type* typeArgs, Location* loc = nullptr);

  /**
   * Constructor.
   *
   * @param name Name.
   * @param loc Location.
   */
  NamedExpression(Name* name, Location* loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~NamedExpression();

  virtual bool isAssignable() const;
  virtual bool isGlobal() const;
  virtual bool isMember() const;
  virtual bool isLocal() const;

  virtual Expression* accept(Cloner* visitor) const;
  virtual Expression* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  /**
   * The category of the identifier.
   */
  ExpressionCategory category;

  /**
   * Once resolved, the unique number of the referent.
   */
  int number;
};
}
