/**
 * @file
 */
#include "bi/expression/BracketsExpression.hpp"

#include "bi/expression/VarParameter.hpp"
#include "bi/visitor/all.hpp"

#include <typeinfo>

bi::BracketsExpression::BracketsExpression(Expression* single,
    Expression* brackets, shared_ptr<Location> loc) :
    Expression(loc), ExpressionUnary(single), Bracketed(brackets) {
  //
}

bi::BracketsExpression::~BracketsExpression() {
  //
}

bi::Expression* bi::BracketsExpression::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::BracketsExpression::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::BracketsExpression::accept(Visitor* visitor) const {
  return visitor->visit(this);
}

bi::possibly bi::BracketsExpression::dispatch(Expression& o) {
  return o.le(*this);
}

bi::possibly bi::BracketsExpression::le(BracketsExpression& o) {
  return *single <= *o.single && *brackets <= *o.brackets;
}

bi::possibly bi::BracketsExpression::le(VarParameter& o) {
  return *type <= *o.type && o.capture(this);
}
