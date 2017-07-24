/**
 * @file
 */
#include "bi/expression/ParenthesesExpression.hpp"

#include "bi/expression/Parameter.hpp"
#include "bi/visitor/all.hpp"

bi::ParenthesesExpression::ParenthesesExpression(Expression* single,
    shared_ptr<Location> loc) :
    Expression(loc),
    Unary<Expression>(single) {
  //
}

bi::ParenthesesExpression::~ParenthesesExpression() {
  //
}

bi::Expression* bi::ParenthesesExpression::strip() {
  return single->strip();
}

bi::Iterator<bi::Expression> bi::ParenthesesExpression::begin() const {
  return single->begin();
}

bi::Iterator<bi::Expression> bi::ParenthesesExpression::end() const {
  return single->end();
}

bi::Expression* bi::ParenthesesExpression::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::ParenthesesExpression::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::ParenthesesExpression::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::ParenthesesExpression::dispatchDefinitely(
    const Expression& o) const {
  return o.definitely(*this) || single->dispatchDefinitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const BracketsExpression& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const EmptyExpression& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<Parameter>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<GlobalVariable>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<LocalVariable>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<MemberVariable>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<Function>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<Coroutine>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<MemberFunction>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<MemberCoroutine>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<BinaryOperator>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const Identifier<UnaryOperator>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Index& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const LambdaFunction& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const List<Expression>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Literal<bool>& o) {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Literal<int64_t>& o) {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Literal<double>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Literal<const char*>& o) {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Member& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Parameter& o) const {
  return (type->definitely(*o.type)) || single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(
    const ParenthesesExpression& o) const {
  return single->definitely(*o.single);
}

bool bi::ParenthesesExpression::definitely(const Range& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Span& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const Super& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::definitely(const This& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesExpression::dispatchPossibly(const Expression& o) const {
  return o.possibly(*this) || single->dispatchPossibly(o);
}

bool bi::ParenthesesExpression::possibly(const BracketsExpression& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const EmptyExpression& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<Parameter>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<GlobalVariable>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<LocalVariable>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<MemberVariable>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<Function>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<Coroutine>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<MemberFunction>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<MemberCoroutine>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<BinaryOperator>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const Identifier<UnaryOperator>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Index& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const LambdaFunction& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const List<Expression>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Literal<bool>& o) {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Literal<int64_t>& o) {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Literal<double>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Literal<const char*>& o) {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Member& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Parameter& o) const {
  return (type->possibly(*o.type)) || single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(
    const ParenthesesExpression& o) const {
  return single->possibly(*o.single);
}

bool bi::ParenthesesExpression::possibly(const Range& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Span& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const Super& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesExpression::possibly(const This& o) const {
  return single->possibly(o);
}
