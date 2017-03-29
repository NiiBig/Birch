/**
 * @file
 */
#include "bi/type/ModelParameter.hpp"

#include "bi/visitor/all.hpp"

#include <typeinfo>

bi::ModelParameter::ModelParameter(shared_ptr<Name> name, Expression* parens,
    shared_ptr<Name> op, Type* base, Expression* braces,
    shared_ptr<Location> loc, const bool assignable) :
    Type(loc, assignable),
    Named(name),
    Parenthesised(parens),
    Based(op, base),
    Braced(braces) {
  //
}

bi::ModelParameter::~ModelParameter() {
  //
}

bi::Type* bi::ModelParameter::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::ModelParameter::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::ModelParameter::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::ModelParameter::isBuiltin() const {
  if (isEqual()) {
    return base->isBuiltin();
  } else {
    return braces->isEmpty();
  }
}

bool bi::ModelParameter::isModel() const {
  if (isEqual()) {
    return base->isModel();
  } else {
    return !braces->isEmpty();
  }
}

bool bi::ModelParameter::isLess() const {
  return !base->isEmpty() && *op == "<";
}

bool bi::ModelParameter::isEqual() const {
  return !base->isEmpty() && *op == "=";
}

bool bi::ModelParameter::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::ModelParameter::definitely(const ModelParameter& o) const {
  return parens->definitely(*o.parens) && base->definitely(*o.base)
      && (!o.assignable || assignable);
}

bool bi::ModelParameter::definitely(const EmptyType& o) const {
  return !o.assignable || assignable;
}

bool bi::ModelParameter::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::ModelParameter::possibly(const ModelParameter& o) const {
  return parens->possibly(*o.parens) && base->possibly(*o.base)
      && (!o.assignable || assignable);
}

bool bi::ModelParameter::possibly(const EmptyType& o) const {
  return !o.assignable || assignable;
}
