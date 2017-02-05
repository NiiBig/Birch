/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"
#include "bi/expression/Expression.hpp"
#include "bi/common/Unary.hpp"

namespace bi {
/**
 * ExpressionStatement.
 *
 * @ingroup compiler_statement
 */
class ExpressionStatement: public Statement, public ExpressionUnary {
public:
  /**
   * Constructor.
   *
   * @param single Expression.
   * @param loc Location.
   */
  ExpressionStatement(Expression* single, shared_ptr<Location> loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~ExpressionStatement();

  virtual Statement* accept(Cloner* visitor) const;
  virtual Statement* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual bool dispatchDefinitely(Statement& o);
  virtual bool definitely(ExpressionStatement& o);

  virtual bool dispatchPossibly(Statement& o);
  virtual bool possibly(ExpressionStatement& o);
};
}
