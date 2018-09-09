/**
 * @file
 */
#include "bi/expression/LocalVariable.hpp"

#include "bi/visitor/all.hpp"

bi::LocalVariable::LocalVariable(const Annotation annotation, Name* name,
    Type* type, Expression* brackets, Expression* args, Expression* value,
    Location* loc) :
    Annotated(annotation),
    Expression(type, loc),
    Named(name),
    Bracketed(brackets),
    Argumented(args),
    Valued(value) {
  //
}

bi::LocalVariable::~LocalVariable() {
  //
}

bool bi::LocalVariable::needsConstruction() const {
  return !args->isEmpty()
      || (value->isEmpty() && (!type->isArray() || !brackets->isEmpty()));
}

bi::Expression* bi::LocalVariable::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::LocalVariable::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::LocalVariable::accept(Visitor* visitor) const {
  visitor->visit(this);
}
