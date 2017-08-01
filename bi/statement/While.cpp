/**
 * @file
 */
#include "bi/statement/While.hpp"

#include "bi/visitor/all.hpp"

bi::While::While(Expression* cond, Statement* braces, shared_ptr<Location> loc) :
    Statement(loc),
    Conditioned(cond),
    Braced(braces) {
  //
}

bi::While::~While() {
  //
}

bi::Statement* bi::While::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Statement* bi::While::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::While::accept(Visitor* visitor) const {
  visitor->visit(this);
}
