/**
 * @file
 */
#include "bi/expression/FuncReference.hpp"

#include "bi/visitor/all.hpp"

#include <vector>
#include <algorithm>
#include <typeinfo>

bi::FuncReference::FuncReference(shared_ptr<Name> name, Expression* parens,
    shared_ptr<Location> loc, const Function* target) :
    Expression(loc),
    Named(name),
    Parenthesised(parens),
    Reference<Function>(target) {
  //
}

bi::FuncReference::~FuncReference() {
  //
}

bi::Expression* bi::FuncReference::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::FuncReference::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::FuncReference::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::FuncReference::dispatchDefinitely(const Expression& o) const {
  return o.definitely(*this);
}

bool bi::FuncReference::definitely(const FuncReference& o) const {
  return parens->definitely(*o.parens);
}

bool bi::FuncReference::definitely(const Function& o) const {
  return parens->definitely(*o.parens);
}

bool bi::FuncReference::definitely(const Parameter& o) const {
  return type->definitely(*o.type);
}

bool bi::FuncReference::dispatchPossibly(const Expression& o) const {
  return o.possibly(*this);
}

bool bi::FuncReference::possibly(const FuncReference& o) const {
  return parens->possibly(*o.parens);
}

bool bi::FuncReference::possibly(const Function& o) const {
  return parens->possibly(*o.parens);
}

bool bi::FuncReference::possibly(const Parameter& o) const {
  return type->possibly(*o.type);
}
