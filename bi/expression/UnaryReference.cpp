/**
 * @file
 */
#include "bi/expression/UnaryReference.hpp"

#include "bi/visitor/all.hpp"

bi::UnaryReference::UnaryReference(shared_ptr<Name> name, Expression* single,
    shared_ptr<Location> loc, const UnaryParameter* target) :
    Expression(loc),
    Named(name),
    Unary<Expression>(single),
    Reference<UnaryParameter>(target) {
  //
}

bi::UnaryReference::~UnaryReference() {
  //
}

bi::Expression* bi::UnaryReference::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::UnaryReference::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::UnaryReference::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::UnaryReference::dispatchDefinitely(const Expression& o) const {
  return o.definitely(*this);
}

bool bi::UnaryReference::definitely(const UnaryReference& o) const {
  return single->definitely(*o.single);
}

bool bi::UnaryReference::definitely(const UnaryParameter& o) const {
  return single->definitely(*o.single);
}

bool bi::UnaryReference::definitely(const VarParameter& o) const {
  return type->definitely(*o.type);
}

bool bi::UnaryReference::dispatchPossibly(const Expression& o) const {
  return o.possibly(*this);
}

bool bi::UnaryReference::possibly(const UnaryReference& o) const {
  return single->possibly(*o.single);
}

bool bi::UnaryReference::possibly(const UnaryParameter& o) const {
  return single->possibly(*o.single);
}

bool bi::UnaryReference::possibly(const VarParameter& o) const {
  return type->possibly(*o.type);
}
