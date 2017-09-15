/**
 * @file
 */
#include "bi/type/AliasType.hpp"

#include "bi/visitor/all.hpp"

bi::AliasType::AliasType(Name* name, Location* loc,
    const bool assignable, Alias* target) :
    Type(loc, assignable),
    Named(name),
    Reference<Alias>(target) {
  //
}

bi::AliasType::AliasType(Alias* target) :
    Named(target->name),
    Reference<Alias>(target) {
  //
}

bi::AliasType::~AliasType() {
  //
}

bi::Type* bi::AliasType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::AliasType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::AliasType::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::AliasType::isBasic() const {
  assert(target);
  return target->base->isBasic();
}

bool bi::AliasType::isClass() const {
  assert(target);
  return target->base->isClass();
}

bool bi::AliasType::isAlias() const {
  return true;
}

bool bi::AliasType::isArray() const {
  assert(target);
  return target->base->isArray();
}

bool bi::AliasType::isFunction() const {
  assert(target);
  return target->base->isFunction();
}

bool bi::AliasType::isFiber() const {
  assert(target);
  return target->base->isFiber();
}

bi::Type* bi::AliasType::resolve(Argumented* args) {
  assert(target);
  return target->base->resolve(args);
}

void bi::AliasType::resolveConstructor(Type* args) {
  assert(target);
  target->base->resolveConstructor(args);
}

bool bi::AliasType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::AliasType::definitely(const AliasType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const ArrayType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const BasicType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const ClassType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const FiberType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const EmptyType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const FunctionType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const ListType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const OptionalType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::definitely(const ParenthesesType& o) const {
  assert(target);
  return target->base->definitely(o);
}

bool bi::AliasType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::AliasType::possibly(const AliasType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const ArrayType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const BasicType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const ClassType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const FiberType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const EmptyType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const FunctionType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const ListType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const OptionalType& o) const {
  assert(target);
  return target->base->possibly(o);
}

bool bi::AliasType::possibly(const ParenthesesType& o) const {
  assert(target);
  return target->base->possibly(o);
}
