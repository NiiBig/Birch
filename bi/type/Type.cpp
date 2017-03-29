/**
 * @file
 */
#include "bi/type/Type.hpp"

#include <cassert>

bi::Type::Type(shared_ptr<Location> loc, const bool assignable) :
    Located(loc),
    assignable(assignable) {
  //
}

bi::Type::~Type() {
  //
}

bool bi::Type::isEmpty() const {
  return false;
}

bool bi::Type::isBuiltin() const {
  return false;
}

bool bi::Type::isModel() const {
  return false;
}

bool bi::Type::isRandom() const {
  return false;
}

bool bi::Type::isLambda() const {
  return false;
}

bool bi::Type::isVariant() const {
  return false;
}

bi::Type* bi::Type::strip() {
  return this;
}

int bi::Type::count() const {
  return 0;
}

bool bi::Type::definitely(Type& o) {
  return o.dispatchDefinitely(*this);
}

bool bi::Type::definitely(AssignableType& o) {
  return false;
}

bool bi::Type::definitely(BracketsType& o) {
  return false;
}

bool bi::Type::definitely(EmptyType& o) {
  return false;
}

bool bi::Type::definitely(LambdaType& o) {
  return false;
}

bool bi::Type::definitely(List<Type>& o) {
  return false;
}

bool bi::Type::definitely(ModelParameter& o) {
  return false;
}

bool bi::Type::definitely(ModelReference& o) {
  return false;
}

bool bi::Type::definitely(ParenthesesType& o) {
  return false;
}

bool bi::Type::definitely(RandomType& o) {
  return false;
}

bool bi::Type::definitely(VariantType& o) {
  return false;
}

bool bi::Type::possibly(Type& o) {
  return o.dispatchPossibly(*this);
}

bool bi::Type::possibly(AssignableType& o) {
  return false;
}

bool bi::Type::possibly(BracketsType& o) {
  return false;
}

bool bi::Type::possibly(EmptyType& o) {
  return false;
}

bool bi::Type::possibly(LambdaType& o) {
  return false;
}

bool bi::Type::possibly(List<Type>& o) {
  return false;
}

bool bi::Type::possibly(ModelParameter& o) {
  return false;
}

bool bi::Type::possibly(ModelReference& o) {
  return false;
}

bool bi::Type::possibly(ParenthesesType& o) {
  return false;
}

bool bi::Type::possibly(RandomType& o) {
  return false;
}

bool bi::Type::possibly(VariantType& o) {
  return false;
}

bool bi::Type::equals(Type& o) {
  return definitely(o) && o.definitely(*this);
}
