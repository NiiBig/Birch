/**
 * @file
 */
#include "bi/statement/MemberFunction.hpp"

#include "bi/visitor/all.hpp"

bi::MemberFunction::MemberFunction(const Annotation annotation, Name* name,
    Expression* params, Type* returnType,
    Statement* braces, Location* loc) :
    Statement(loc),
    Annotated(annotation),
    Named(name),
    Parameterised(params),
    ReturnTyped(returnType),
    Typed(new EmptyType(loc)),
    Scoped(LOCAL_SCOPE),
    Braced(braces) {
  //
}

bi::MemberFunction::~MemberFunction() {
  //
}

bi::Statement* bi::MemberFunction::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Statement* bi::MemberFunction::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::MemberFunction::accept(Visitor* visitor) const {
  visitor->visit(this);
}
