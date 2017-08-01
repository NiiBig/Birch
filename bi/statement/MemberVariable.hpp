/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Numbered.hpp"
#include "bi/common/Typed.hpp"
#include "bi/common/Parenthesised.hpp"
#include "bi/common/Valued.hpp"
#include "bi/primitive/unique_ptr.hpp"

namespace bi {
/**
 * Class member variable.
 *
 * @ingroup compiler_statement
 */
class MemberVariable: public Statement,
    public Named,
    public Numbered,
    public Typed,
    public Parenthesised,
    public Valued {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param type Type.
   * @param parens Constructor arguments.
   * @param value Initial value.
   * @param loc Location.
   */
  MemberVariable(shared_ptr<Name> name, Type* type, Expression* parens =
      new EmptyExpression(), Expression* value = new EmptyExpression(),
      shared_ptr<Location> loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~MemberVariable();

  virtual Statement* accept(Cloner* visitor) const;
  virtual Statement* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;
};
}
