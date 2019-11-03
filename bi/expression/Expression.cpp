/**
 * @file
 */
#include "bi/expression/Expression.hpp"

#include "bi/expression/ExpressionIterator.hpp"
#include "bi/expression/ExpressionConstIterator.hpp"
#include "bi/type/EmptyType.hpp"
#include "bi/expression/Range.hpp"
#include "bi/exception/NotFunctionException.hpp"
#include "bi/visitor/all.hpp"

bi::Expression::Expression(Type* type, Location* loc) :
    Located(loc),
    Typed(type) {
  //
}

bi::Expression::Expression(Location* loc) :
    Located(loc),
    Typed(new EmptyType(loc)) {
  //
}

bi::Expression::~Expression() {
  //
}

bool bi::Expression::isValue() const {
  IsValue visitor;
  accept(&visitor);
  return visitor.result;
}

bool bi::Expression::isEmpty() const {
  return false;
}

bool bi::Expression::isAssignable() const {
  return false;
}

bi::Expression* bi::Expression::strip() {
  return this;
}

bi::Expression* bi::Expression::getLeft() const {
  assert(false);
  return nullptr;
}

bi::Expression* bi::Expression::getRight() const {
  assert(false);
  return nullptr;
}

int bi::Expression::width() const {
  int result = 0;
  for (auto iter = begin(); iter != end(); ++iter) {
    ++result;
  }
  return result;
}

int bi::Expression::depth() const {
  int result = 0;
  for (auto iter = begin(); iter != end(); ++iter) {
    if (dynamic_cast<const Range*>(*iter)) {
      ++result;
    }
  }
  return result;
}

bi::ExpressionIterator bi::Expression::begin() {
  if (isEmpty()) {
    return end();
  } else {
    return ExpressionIterator(this);
  }
}

bi::ExpressionIterator bi::Expression::end() {
  return ExpressionIterator(nullptr);
}

bi::ExpressionConstIterator bi::Expression::begin() const {
  if (isEmpty()) {
    return end();
  } else {
    return ExpressionConstIterator(this);
  }
}

bi::ExpressionConstIterator bi::Expression::end() const {
  return ExpressionConstIterator(nullptr);
}

bi::Lookup bi::Expression::lookup(Expression* args) {
  throw NotFunctionException(this);
}

bi::Parameter* bi::Expression::resolve(Call<Parameter>* o) {
  assert(false);
}

bi::LocalVariable* bi::Expression::resolve(Call<LocalVariable>* o) {
  assert(false);
}

bi::MemberVariable* bi::Expression::resolve(Call<MemberVariable>* o) {
  assert(false);
}

bi::GlobalVariable* bi::Expression::resolve(Call<GlobalVariable>* o) {
  assert(false);
}

bi::Function* bi::Expression::resolve(Call<Function>* o) {
  assert(false);
}

bi::MemberFunction* bi::Expression::resolve(Call<MemberFunction>* o) {
  assert(false);
}

bi::Fiber* bi::Expression::resolve(Call<Fiber>* o) {
  assert(false);
}

bi::MemberFiber* bi::Expression::resolve(Call<MemberFiber>* o) {
  assert(false);
}

bi::UnaryOperator* bi::Expression::resolve(Call<UnaryOperator>* o) {
  assert(false);
}

bi::BinaryOperator* bi::Expression::resolve(Call<BinaryOperator>* o) {
  assert(false);
}
