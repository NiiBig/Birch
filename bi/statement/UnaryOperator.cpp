/**
 * @file
 */
#include "bi/statement/UnaryOperator.hpp"

#include "bi/visitor/all.hpp"

bi::UnaryOperator::UnaryOperator(Name* name, Expression* typeParams,
    Expression* params, Type* returnType, Statement* braces, Location* loc) :
    Statement(loc),
    Named(name),
    TypeParameterised(typeParams),
    Parameterised(params),
    ReturnTyped(returnType),
    Typed(new EmptyType(loc)),
    Scoped(LOCAL_SCOPE),
    Braced(braces) {
  //
}

bi::UnaryOperator::~UnaryOperator() {
  //
}

bi::Statement* bi::UnaryOperator::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Statement* bi::UnaryOperator::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::UnaryOperator::accept(Visitor* visitor) const {
  visitor->visit(this);
}
