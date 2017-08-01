/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Numbered.hpp"
#include "bi/common/Parenthesised.hpp"
#include "bi/common/ReturnTyped.hpp"
#include "bi/common/Braced.hpp"
#include "bi/common/Scoped.hpp"

namespace bi {
/**
 * Coroutine.
 *
 * @ingroup compiler_statement
 */
class Coroutine: public Statement,
    public Named,
    public Numbered,
    public Parenthesised,
    public ReturnTyped,
    public Typed,
    public Scoped,
    public Braced {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param parens Parentheses expression.
   * @param returnType Return type.
   * @param braces Braces expression.
   * @param loc Location.
   */
  Coroutine(Name* name, Expression* parens, Type* returnType,
      Statement* braces, Location* loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~Coroutine();

  virtual Statement* accept(Cloner* visitor) const;
  virtual Statement* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;
};
}
