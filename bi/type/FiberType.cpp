/**
 * @file
 */
#include "bi/type/FiberType.hpp"

#include "bi/visitor/all.hpp"

bi::FiberType::FiberType(Type* single, Location* loc, const bool assignable) :
    Type(loc, assignable),
    Single<Type>(single) {
  //
}

bi::FiberType::~FiberType() {
  //
}

bi::Type* bi::FiberType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::FiberType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::FiberType::accept(Visitor* visitor) const {
  return visitor->visit(this);
}

bool bi::FiberType::isFiber() const {
  return true;
}

bool bi::FiberType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::FiberType::definitely(const AliasType& o) const {
  return definitely(*o.target->base);
}

bool bi::FiberType::definitely(const FiberType& o) const {
  return single->definitely(*o.single);
}

bool bi::FiberType::definitely(const OptionalType& o) const {
  return definitely(*o.single);
}

bool bi::FiberType::definitely(const ParenthesesType& o) const {
  return definitely(*o.single);
}

bool bi::FiberType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::FiberType::possibly(const AliasType& o) const {
  return possibly(*o.target->base);
}

bool bi::FiberType::possibly(const FiberType& o) const {
  return single->possibly(*o.single);
}

bool bi::FiberType::possibly(const OptionalType& o) const {
  return possibly(*o.single);
}

bool bi::FiberType::possibly(const ParenthesesType& o) const {
  return possibly(*o.single);
}

