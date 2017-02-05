/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"
#include "bi/expression/VarParameter.hpp"

namespace bi {
/**
 * Self-reference to an object.
 *
 * @ingroup compiler_expression
 */
class This: public Expression {
public:
  /**
   * Constructor.
   *
   * @param loc Location.
   */
  This(shared_ptr<Location> loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~This();

  virtual Expression* accept(Cloner* visitor) const;
  virtual Expression* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual bool dispatchDefinitely(Expression& o);
  virtual bool definitely(This& o);
  virtual bool definitely(VarParameter& o);

  virtual bool dispatchPossibly(Expression& o);
  virtual bool possibly(This& o);
  virtual bool possibly(VarParameter& o);
};
}
