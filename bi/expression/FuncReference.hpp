/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"
#include "bi/expression/FuncParameter.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Reference.hpp"
#include "bi/common/Parenthesised.hpp"
#include "bi/common/Formed.hpp"
#include "bi/expression/VarParameter.hpp"

#include <list>

namespace bi {
/**
 * Reference to function.
 *
 * @ingroup compiler_expression
 */
class FuncReference: public Expression, public Named, public Reference<
    FuncParameter>, public Parenthesised, public Formed {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param parens Expression in parentheses.
   * @param form Function form.
   * @param loc Location.
   * @param target Target.
   */
  FuncReference(shared_ptr<Name> name, Expression* parens,
      const FunctionForm form, shared_ptr<Location> loc = nullptr,
      const FuncParameter* target = nullptr);

  /**
   * Constructor for binary operator.
   *
   * @param left Left operand.
   * @param name Operator.
   * @param right Right operand.
   * @param loc Location.
   * @param target Target.
   */
  FuncReference(Expression* left, shared_ptr<Name> name, Expression* right,
      shared_ptr<Location> loc = nullptr, const FuncParameter* target =
          nullptr);

  /**
   * Destructor.
   */
  virtual ~FuncReference();

  virtual Expression* accept(Cloner* visitor) const;
  virtual Expression* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  /**
   * Arguments.
   */
  std::list<const Expression*> args;

  virtual bool dispatch(Expression& o);
  virtual bool le(FuncParameter& o);
  virtual bool le(FuncReference& o);
  virtual bool le(VarParameter& o);
  virtual bool le(VarReference& o);
};
}
