/**
 * @file
 */
#include "bi/statement/For.hpp"

#include "bi/visitor/all.hpp"

bi::For::For(Expression* index, Expression* from, Expression* to,
    Statement* braces, Location* loc) :
    Statement(loc),
    Braced(braces),
    index(index),
    from(from),
    to(to) {
  //
}

bi::For::~For() {
  //
}

bi::Statement* bi::For::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Statement* bi::For::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::For::accept(Visitor* visitor) const {
  visitor->visit(this);
}
