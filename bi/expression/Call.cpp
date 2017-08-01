/**
 * @file
 */
#include "bi/expression/Call.hpp"

#include "bi/visitor/all.hpp"

bi::Call::Call(Expression* single, Expression* args, Location* loc) :
    Expression(loc),
    Single<Expression>(single),
    Argumented(args) {
  //
}

bi::Call::~Call() {
  //
}

bi::Expression* bi::Call::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::Call::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::Call::accept(Visitor* visitor) const {
  return visitor->visit(this);
}
