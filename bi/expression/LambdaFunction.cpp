/**
 * @file
 */
#include "bi/expression/LambdaFunction.hpp"

#include "bi/visitor/all.hpp"

bi::LambdaFunction::LambdaFunction(Expression* params, Type* returnType,
    Statement* braces, Location* loc) :
    Expression(loc),
    Parameterised(params),
    ReturnTyped(returnType),
    Braced(braces) {
  //
}

bi::LambdaFunction::~LambdaFunction() {
  //
}

bi::Expression* bi::LambdaFunction::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::LambdaFunction::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::LambdaFunction::accept(Visitor* visitor) const {
  visitor->visit(this);
}
