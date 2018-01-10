/**
 * @file
 */
#include "bi/type/TupleType.hpp"

#include "bi/visitor/all.hpp"

bi::TupleType::TupleType(Type* single, Location* loc) :
    Type(loc),
    Single<Type>(single) {
  //
}

bi::TupleType::~TupleType() {
  //
}

bi::Type* bi::TupleType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::TupleType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::TupleType::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::TupleType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::TupleType::definitely(const GenericType& o) const {
  assert(o.target);
  return definitely(*o.target->type);
}

bool bi::TupleType::definitely(const OptionalType& o) const {
  return definitely(*o.single);
}

bool bi::TupleType::definitely(const TupleType& o) const {
  return single->definitely(*o.single);
}

bool bi::TupleType::definitely(const AnyType& o) const {
  return true;
}

bi::Type* bi::TupleType::dispatchCommon(const Type& o) const {
  return o.common(*this);
}

bi::Type* bi::TupleType::common(const GenericType& o) const {
  assert(o.target);
  return common(*o.target->type);
}

bi::Type* bi::TupleType::common(const OptionalType& o) const {
  auto single1 = common(*o.single);
  if (single1) {
    return new OptionalType(single1);
  } else {
    return nullptr;
  }
}

bi::Type* bi::TupleType::common(const TupleType& o) const {
  auto single1 = single->common(*o.single);
  if (single1) {
    return new TupleType(single1);
  } else {
    return nullptr;
  }
}

bi::Type* bi::TupleType::common(const AnyType& o) const {
  return new AnyType();
}
