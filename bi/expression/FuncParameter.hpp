/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"
#include "bi/common/Braced.hpp"
#include "bi/common/Signature.hpp"
#include "bi/common/Scoped.hpp"

namespace bi {
/**
 * Function parameter.
 *
 * @ingroup compiler_expression
 */
class FuncParameter: public Expression,
    public Signature,
    public Scoped,
    public Braced {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param parens Parentheses expression.
   * @param type Return type.
   * @param braces Braces expression.
   * @param form Function form.
   * @param loc Location.
   */
  FuncParameter(shared_ptr<Name> name, Expression* parens, Type* type,
      Expression* braces, const FunctionForm form, shared_ptr<Location> loc =
          nullptr);

  /**
   * Constructor for binary operator.
   *
   * @param left Left operand.
   * @param name Operator.
   * @param right Right operand.
   * @param type Return type.
   * @param braces Braces expression.
   * @param form Signature form.
   * @param loc Location.
   */
  FuncParameter(Expression* left, shared_ptr<Name> name, Expression* right,
      Type* type, Expression* braces, const FunctionForm form,
      shared_ptr<Location> loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~FuncParameter();

  virtual Expression* accept(Cloner* visitor) const;
  virtual Expression* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  using Expression::definitely;
  using Expression::possibly;

  virtual bool dispatchDefinitely(const Expression& o) const;
  virtual bool definitely(const FuncParameter& o) const;

  virtual bool dispatchPossibly(const Expression& o) const;
  virtual bool possibly(const FuncParameter& o) const;
};
}
