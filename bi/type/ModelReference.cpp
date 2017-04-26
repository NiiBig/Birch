/**
 * @file
 */
#include "bi/type/ModelReference.hpp"

#include "bi/visitor/all.hpp"

bi::ModelReference::ModelReference(shared_ptr<Name> name, Expression* parens,
    shared_ptr<Location> loc, const bool assignable, ModelParameter* target) :
    Type(loc, assignable),
    Named(name),
    Parenthesised(parens),
    Reference(target) {
  //
}

bi::ModelReference::ModelReference(ModelParameter* target) :
    Named(target->name),
    Reference(target) {
  //
}

bi::ModelReference::~ModelReference() {
  //
}

bi::Type* bi::ModelReference::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::ModelReference::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::ModelReference::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::ModelReference::isBuiltin() const {
  /* pre-condition */
  assert(target);

  return target->isBuiltin();
}

bool bi::ModelReference::isStruct() const {
  /* pre-condition */
  assert(target);

  return target->isStruct();
}

bool bi::ModelReference::isClass() const {
  /* pre-condition */
  assert(target);

  return target->isClass();
}

bool bi::ModelReference::isAlias() const {
  /* pre-condition */
  assert(target);

  return target->isAlias();
}

bool bi::ModelReference::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::ModelReference::definitely(const ModelParameter& o) const {
  return true;
}

bool bi::ModelReference::definitely(const ModelReference& o) const {
  return target->canUpcast(o.target) && (!o.assignable || assignable);
}

bool bi::ModelReference::definitely(const ParenthesesType& o) const {
  return definitely(*o.single) && (!o.assignable || assignable);
}

bool bi::ModelReference::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::ModelReference::possibly(const ModelParameter& o) const {
  return true;
}

bool bi::ModelReference::possibly(const ModelReference& o) const {
  /* pre-condition */
  assert(target && o.target);

  return target->canDowncast(o.target) && (!o.assignable || assignable);
}

bool bi::ModelReference::possibly(const ParenthesesType& o) const {
  return possibly(*o.single) && (!o.assignable || assignable);
}
