/**
 * @file
 */
#include "bi/type/BasicType.hpp"

#include "bi/visitor/all.hpp"

bi::BasicType::BasicType(Name* name, Location* loc,
    const bool assignable, Basic* target) :
    Type(loc, assignable),
    Named(name),
    Reference<Basic>(target) {
  //
}

bi::BasicType::BasicType(Basic* target) :
    Named(target->name),
    Reference<Basic>(target) {
  //
}

bi::BasicType::~BasicType() {
  //
}

bi::Type* bi::BasicType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::BasicType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::BasicType::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::BasicType::isBasic() const {
  return true;
}

bool bi::BasicType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::BasicType::definitely(const AliasType& o) const {
  return definitely(*o.target->base);
}

bool bi::BasicType::definitely(const BasicType& o) const {
  return target == o.target;
}

bool bi::BasicType::definitely(const OptionalType& o) const {
  return definitely(*o.single);
}

bool bi::BasicType::definitely(const ParenthesesType& o) const {
  return definitely(*o.single);
}

bool bi::BasicType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::BasicType::possibly(const AliasType& o) const {
  return possibly(*o.target->base);
}

bool bi::BasicType::possibly(const BasicType& o) const {
  return false;
}

bool bi::BasicType::possibly(const OptionalType& o) const {
  return possibly(*o.single);
}

bool bi::BasicType::possibly(const ParenthesesType& o) const {
  return possibly(*o.single);
}
