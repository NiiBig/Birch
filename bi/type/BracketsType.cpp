/**
 * @file
 */
#include "bi/type/BracketsType.hpp"

#include "bi/visitor/all.hpp"

bi::BracketsType::BracketsType(Type* single, Expression* brackets,
    shared_ptr<Location> loc, const bool assignable, const bool polymorphic) :
    Type(loc, assignable, polymorphic),
    TypeUnary(single),
    Bracketed(brackets),
    ndims(brackets->tupleSize()) {
  //
}

bi::BracketsType::BracketsType(Type* single, const int ndims) :
    TypeUnary(single),
    ndims(ndims) {
  //
}

bi::BracketsType::~BracketsType() {
  //
}

int bi::BracketsType::count() const {
  return ndims;
}

bool bi::BracketsType::isArray() const {
  return true;
}

bi::Type* bi::BracketsType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::BracketsType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::BracketsType::accept(Visitor* visitor) const {
  return visitor->visit(this);
}

bool bi::BracketsType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::BracketsType::definitely(const BracketsType& o) const {
  return single->definitely(*o.single) && brackets->definitely(*o.brackets)
      && (!o.assignable || assignable);
}

bool bi::BracketsType::definitely(const ParenthesesType& o) const {
  return definitely(*o.single) && (!o.assignable || assignable);
}

bool bi::BracketsType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::BracketsType::possibly(const BracketsType& o) const {
  return single->possibly(*o.single) && brackets->possibly(*o.brackets)
      && (!o.assignable || assignable);
}

bool bi::BracketsType::possibly(const ParenthesesType& o) const {
  return possibly(*o.single) && (!o.assignable || assignable);
}
