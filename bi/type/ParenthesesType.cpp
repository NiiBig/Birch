/**
 * @file
 */
#include "bi/type/ParenthesesType.hpp"

#include "bi/visitor/all.hpp"

#include <typeinfo>

bi::ParenthesesType::ParenthesesType(Type* single, shared_ptr<Location> loc,
    const bool assignable) :
    Type(loc, assignable),
    Unary<Type>(single) {
  //
}

bi::ParenthesesType::~ParenthesesType() {
  //
}

bool bi::ParenthesesType::isBasic() const {
  return single->isBasic();
}

bool bi::ParenthesesType::isClass() const {
  return single->isClass();
}

bool bi::ParenthesesType::isAlias() const {
  return single->isAlias();
}

bool bi::ParenthesesType::isFunction() const {
  return single->isFunction();
}

bool bi::ParenthesesType::isCoroutine() const {
  return single->isFunction();
}

bi::Type* bi::ParenthesesType::strip() {
  return single->strip();
}

bi::Type* bi::ParenthesesType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::ParenthesesType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::ParenthesesType::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::ParenthesesType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::ParenthesesType::definitely(const ArrayType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const FiberType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const EmptyType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const FunctionType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const List<Type>& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const ClassType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const AliasType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const BasicType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const ParenthesesType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::ParenthesesType::possibly(const ArrayType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const FiberType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const EmptyType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const FunctionType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const List<Type>& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const ClassType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const AliasType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const BasicType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const ParenthesesType& o) const {
  return single->possibly(*o.single);
}
