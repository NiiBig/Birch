/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"
#include "bi/statement/Statement.hpp"
#include "bi/common/Unary.hpp"
#include "bi/primitive/unique_ptr.hpp"

namespace bi {
/**
 * Braces.
 *
 * @ingroup compiler_expression
 */
class BracesExpression: public Expression, public StatementUnary {
public:
  /**
   * Constructor.
   *
   * @param single Statement in braces.
   * @param loc Location.
   */
  BracesExpression(Statement* single, shared_ptr<Location> loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~BracesExpression();

  virtual Expression* accept(Cloner* visitor) const;
  virtual Expression* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  using Expression::definitely;
  using Expression::possibly;

  virtual bool dispatchDefinitely(Expression& o);
  virtual bool definitely(BracesExpression& o);

  virtual bool dispatchPossibly(Expression& o);
  virtual bool possibly(BracesExpression& o);
};
}
