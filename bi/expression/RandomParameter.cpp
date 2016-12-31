/**
 * @file
 */
#include "bi/expression/RandomParameter.hpp"

#include "bi/expression/VarParameter.hpp"
#include "bi/visitor/all.hpp"
#include "bi/primitive/encode.hpp"

#include <typeinfo>

bi::RandomParameter::RandomParameter(Expression* left, shared_ptr<Name> op,
    Expression* right, shared_ptr<Location> loc) :
    Expression(loc),
    ExpressionBinary(left, right),
    op(op) {
  name = new Name(uniqueName(left));
}

bi::RandomParameter::RandomParameter(FuncReference* ref) :
    Expression(ref->loc),
    ExpressionBinary(ref->releaseLeft(), ref->releaseRight()),
    op(ref->name) {
  name = new Name(uniqueName(left.get()));
}

bi::RandomParameter::~RandomParameter() {
  //
}

bi::Expression* bi::RandomParameter::acceptClone(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::RandomParameter::acceptModify(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::RandomParameter::accept(Visitor* visitor) const {
  return visitor->visit(this);
}

bool bi::RandomParameter::operator<=(Expression& o) {
  try {
    RandomParameter& o1 = dynamic_cast<RandomParameter&>(o);
    return *left <= *o1.left && *op == *o1.op && *right <= *o1.right;
  } catch (std::bad_cast e) {
    //
  }
  try {
    VarParameter& o1 = dynamic_cast<VarParameter&>(o);
    return *left->type <= *o1.type && o1.capture(left.get());
  } catch (std::bad_cast e) {
    //
  }
  try {
    VarReference& o1 = dynamic_cast<VarReference&>(o);
    return *left->type <= *o1.type && o1.check(left.get());
  } catch (std::bad_cast e) {
    //
  }
  try {
    ParenthesesExpression& o1 = dynamic_cast<ParenthesesExpression&>(o);
    return *this <= *o1.expr;
  } catch (std::bad_cast e) {
    //
  }
  return false;
}

bool bi::RandomParameter::operator==(const Expression& o) const {
  try {
    const RandomParameter& o1 = dynamic_cast<const RandomParameter&>(o);
    return *left == *o1.left && *op == *o1.op && *right == *o1.right;
  } catch (std::bad_cast e) {
    //
  }
  return false;
}
