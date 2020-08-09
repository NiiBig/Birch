/**
 * @file
 */
#include "bi/statement/AssignmentOperator.hpp"

#include "bi/visitor/all.hpp"

bi::AssignmentOperator::AssignmentOperator(Expression* single,
    Statement* braces, Location* loc) :
    Statement(loc),
    Single(single),
    Scoped(LOCAL_SCOPE),
    Braced(braces) {
  //
}

bi::AssignmentOperator::~AssignmentOperator() {
  //
}

bool bi::AssignmentOperator::isDeclaration() const {
  return true;
}

bi::Statement* bi::AssignmentOperator::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Statement* bi::AssignmentOperator::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::AssignmentOperator::accept(Visitor* visitor) const {
  visitor->visit(this);
}
