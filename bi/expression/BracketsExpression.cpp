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

bool bi::BracketsExpression::dispatchDefinitely(const Expression& o) const {
  return o.definitely(*this);
}

bool bi::BracketsExpression::definitely(const BracketsExpression& o) const {
  return single->definitely(*o.single) && brackets->definitely(*o.brackets);
}

bool bi::BracketsExpression::definitely(const VarParameter& o) const {
  return type->definitely(*o.type);
}

bool bi::BracketsExpression::dispatchPossibly(const Expression& o) const {
  return o.possibly(*this);
}

bool bi::BracketsExpression::possibly(const BracketsExpression& o) const {
  return single->possibly(*o.single) && brackets->possibly(*o.brackets);
}

bool bi::BracketsExpression::possibly(const VarParameter& o) const {
  return type->possibly(*o.type);
}
