/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"
#include "bi/expression/Expression.hpp"
#include "bi/common/Single.hpp"
#include "bi/statement/LocalVariable.hpp"

#include <list>

namespace bi {
/**
 * Yield statement.
 *
 * @ingroup statement
 */
class Yield: public Statement, public Single<Expression> {
public:
  /**
   * Constructor.
   *
   * @param single Expression.
   * @param loc Location.
   */
  Yield(Expression* single, Location* loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~Yield();

  virtual Statement* accept(Cloner* visitor) const;
  virtual Statement* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  /**
   * Resume function.
   */
  Statement* resume;

  /**
   * Local variables in scope at this statement.
   */
  std::list<LocalVariable*> locals;
};
}
