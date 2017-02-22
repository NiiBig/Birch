/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"
#include "bi/common/Braced.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Numbered.hpp"
#include "bi/common/Scoped.hpp"
#include "bi/common/Formed.hpp"
#include "bi/common/Parameter.hpp"

namespace bi {
/**
 * Function parameter.
 *
 * @ingroup compiler_expression
 */
class FuncParameter: public Expression,
    public Named,
    public Numbered,
    public Scoped,
    public Formed,
    public Braced,
    public Parameter<Expression> {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param parens Parentheses expression.
   * @param result Result expression.
   * @param braces Braces expression.
   * @param form Function form.
   * @param loc Location.
   */
  FuncParameter(shared_ptr<Name> name, Expression* parens, Expression* result,
      Expression* braces, const FunctionForm form = FUNCTION,
      shared_ptr<Location> loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~FuncParameter();

  virtual Expression* accept(Cloner* visitor) const;
  virtual Expression* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  /**
   * Result expression.
   */
  unique_ptr<Expression> result;

  /**
   * Mangled name.
   */
  shared_ptr<Name> mangled;

  using Expression::definitely;
  using Expression::possibly;

  virtual bool dispatchDefinitely(Expression& o);
  virtual bool definitely(FuncParameter& o);

  virtual bool dispatchPossibly(Expression& o);
  virtual bool possibly(FuncParameter& o);
};
}
