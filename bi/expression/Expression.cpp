/**
 * @file
 */
#include "bi/expression/Expression.hpp"

#include "bi/common/Iterator.hpp"
#include "bi/type/EmptyType.hpp"
#include "bi/expression/Range.hpp"

bi::Expression::Expression(Type* type, shared_ptr<Location> loc) :
    Located(loc),
    Typed(type) {
  //
}

bi::Expression::Expression(shared_ptr<Location> loc) :
    Located(loc) {
  //
}

bi::Expression::~Expression() {
  //
}

bool bi::Expression::isEmpty() const {
  return false;
}

bi::Expression* bi::Expression::strip() {
  return this;
}

int bi::Expression::tupleSize() const {
  int result = 0;
  for (auto iter = begin(); iter != end(); ++iter) {
    ++result;
  }
  return result;
}

int bi::Expression::tupleDims() const {
  int result = 0;
  for (auto iter = begin(); iter != end(); ++iter) {
    if (dynamic_cast<const Range*>(*iter)) {
      ++result;
    }
  }
  return result;
}

bi::Iterator<bi::Expression> bi::Expression::begin() const {
  if (isEmpty()) {
    return end();
  } else {
    return bi::Iterator<Expression>(this);
  }
}

bi::Iterator<bi::Expression> bi::Expression::end() const {
  return bi::Iterator<Expression>(nullptr);
}
