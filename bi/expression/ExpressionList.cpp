/**
 * @file
 */
#include "bi/expression/ExpressionList.hpp"

#include "bi/visitor/all.hpp"

bi::ExpressionList::ExpressionList(Expression* head, Expression* tail,
    Location* loc) :
    Expression(loc),
    head(head),
    tail(tail) {
  /* pre-conditions */
  assert(head);
  assert(tail);

  this->loc = loc;
}

bi::ExpressionList::~ExpressionList() {
  //
}

int bi::ExpressionList::width() const {
  const ExpressionList* listTail = dynamic_cast<const ExpressionList*>(tail);
  if (listTail) {
    return 1 + listTail->width();
  } else {
    return 2;
  }
}

bool bi::ExpressionList::isAssignable() const {
  return head->isAssignable() && tail->isAssignable();
}

bi::Expression* bi::ExpressionList::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Expression* bi::ExpressionList::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::ExpressionList::accept(Visitor* visitor) const {
  visitor->visit(this);
}
