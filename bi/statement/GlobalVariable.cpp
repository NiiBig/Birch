/**
 * @file
 */
#include "bi/statement/GlobalVariable.hpp"

#include "bi/visitor/all.hpp"

bi::GlobalVariable::GlobalVariable(shared_ptr<Name> name, Type* type,
    Expression* parens, Expression* value, shared_ptr<Location> loc) :
    Statement(loc),
    Named(name),
    Typed(type),
    Parenthesised(parens),
    Valued(value) {
  //
}

bi::GlobalVariable::~GlobalVariable() {
  //
}

bi::Statement* bi::GlobalVariable::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Statement* bi::GlobalVariable::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::GlobalVariable::accept(Visitor* visitor) const {
  visitor->visit(this);
}
