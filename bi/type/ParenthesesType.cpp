/**
 * @file
 */
#include "bi/type/ParenthesesType.hpp"

#include "bi/visitor/all.hpp"

bi::ParenthesesType::ParenthesesType(Type* single, Location* loc,
    const bool assignable) :
    Type(loc, assignable),
    Single<Type>(single) {
  //
}

bi::ParenthesesType::~ParenthesesType() {
  //
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

bool bi::ParenthesesType::definitely(const AliasType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const ArrayType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const BasicType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const BinaryType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const ClassType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const EmptyType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const FiberType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const FunctionType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const ListType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const NilType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const OptionalType& o) const {
  return single->definitely(o);
}

bool bi::ParenthesesType::definitely(const ParenthesesType& o) const {
  return single->definitely(*o.single);
}

bool bi::ParenthesesType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::ParenthesesType::possibly(const AliasType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const ArrayType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const BasicType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const BinaryType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const ClassType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const EmptyType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const FiberType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const FunctionType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const ListType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const NilType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const OptionalType& o) const {
  return single->possibly(o);
}

bool bi::ParenthesesType::possibly(const ParenthesesType& o) const {
  return single->possibly(*o.single);
}
