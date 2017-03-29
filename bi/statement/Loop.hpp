/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"
#include "bi/common/Conditioned.hpp"
#include "bi/common/Braced.hpp"
#include "bi/common/Scoped.hpp"

namespace bi {
/**
 * Loop.
 *
 * @ingroup compiler_statement
 */
class Loop: public Statement, public Conditioned, public Braced, public Scoped {
public:
  /**
   * Constructor.
   *
   * @param cond Condition.
   * @param braces Body of loop.
   * @param loc Location.
   */
  Loop(Expression* cond, Expression* braces,
      shared_ptr<Location> loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~Loop();

  virtual Statement* accept(Cloner* visitor) const;
  virtual Statement* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  using Statement::definitely;
  using Statement::possibly;

  virtual bool dispatchDefinitely(const Statement& o) const;
  virtual bool definitely(const Loop& o) const;

  virtual bool dispatchPossibly(const Statement& o) const;
  virtual bool possibly(const Loop& o) const;
};
}
