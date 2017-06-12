/**
 * @file
 */
#include "bi/expression/LambdaFunction.hpp"

#include "bi/visitor/all.hpp"

bi::LambdaFunction::LambdaFunction(Expression* parens, Type* returnType,
    Expression* braces, shared_ptr<Location> loc) :
    Expression(loc),
    Parenthesised(parens),
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

bool bi::LambdaFunction::dispatchDefinitely(const Expression& o) const {
  return o.definitely(*this);
}

bool bi::LambdaFunction::definitely(const LambdaFunction& o) const {
  return parens->definitely(*o.parens);
}

bool bi::LambdaFunction::dispatchPossibly(const Expression& o) const {
  return o.possibly(*this);
}

bool bi::LambdaFunction::possibly(const LambdaFunction& o) const {
  return parens->possibly(*o.parens);
}
