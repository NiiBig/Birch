/**
 * @file
 */
#include "bi/visitor/Visitor.hpp"

bi::Visitor::~Visitor() {
  //
}

void bi::Visitor::visit(const Name* o) {
  //
}

void bi::Visitor::visit(const Path* o) {
  o->head->accept(this);
  if (o->tail) {
    o->tail->accept(this);
  }
}

void bi::Visitor::visit(const EmptyExpression* o) {
  //
}

void bi::Visitor::visit(const EmptyStatement* o) {
  //
}

void bi::Visitor::visit(const EmptyType* o) {
  //
}

void bi::Visitor::visit(const BooleanLiteral* o) {
  o->type->accept(this);
}

void bi::Visitor::visit(const IntegerLiteral* o) {
  o->type->accept(this);
}

void bi::Visitor::visit(const RealLiteral* o) {
  o->type->accept(this);
}

void bi::Visitor::visit(const StringLiteral* o) {
  o->type->accept(this);
}

void bi::Visitor::visit(const ExpressionList* o) {
  o->head->accept(this);
  o->tail->accept(this);
}

void bi::Visitor::visit(const StatementList* o) {
  o->head->accept(this);
  o->tail->accept(this);
}

void bi::Visitor::visit(const ParenthesesExpression* o) {
  o->single->accept(this);
}

void bi::Visitor::visit(const BracesExpression* o) {
  o->single->accept(this);
}

void bi::Visitor::visit(const BracketsExpression* o) {
  o->single->accept(this);
  o->brackets->accept(this);
}

void bi::Visitor::visit(const Dispatcher* o) {
  o->parens->accept(this);
}

void bi::Visitor::visit(const Index* o) {
  o->single->accept(this);
}

void bi::Visitor::visit(const Range* o) {
  o->left->accept(this);
  o->right->accept(this);
}

void bi::Visitor::visit(const This* o) {
  //
}

void bi::Visitor::visit(const Member* o) {
  o->left->accept(this);
  o->right->accept(this);
}

void bi::Visitor::visit(const VarReference* o) {
  //
}

void bi::Visitor::visit(const FuncReference* o) {
  o->parens->accept(this);
}

void bi::Visitor::visit(const ModelReference* o) {
  o->parens->accept(this);
}

void bi::Visitor::visit(const ProgReference* o) {
  o->parens->accept(this);
}

void bi::Visitor::visit(const VarParameter* o) {
  o->type->accept(this);
  o->value->accept(this);
}

void bi::Visitor::visit(const FuncParameter* o) {
  o->parens->accept(this);
  o->result->accept(this);
  o->braces->accept(this);
}

void bi::Visitor::visit(const ModelParameter* o) {
  o->parens->accept(this);
  o->base->accept(this);
  o->braces->accept(this);
}

void bi::Visitor::visit(const ProgParameter* o) {
  o->parens->accept(this);
  o->braces->accept(this);
}

void bi::Visitor::visit(const File* o) {
  o->root->accept(this);
}

void bi::Visitor::visit(const Import* o) {
  o->path->accept(this);
}

void bi::Visitor::visit(const ExpressionStatement* o) {
  o->single->accept(this);
}

void bi::Visitor::visit(const Conditional* o) {
  o->cond->accept(this);
  o->braces->accept(this);
  o->falseBraces->accept(this);
}

void bi::Visitor::visit(const Loop* o) {
  o->cond->accept(this);
  o->braces->accept(this);
}

void bi::Visitor::visit(const Raw* o) {
  //
}

void bi::Visitor::visit(const VarDeclaration* o) {
  o->param->accept(this);
}

void bi::Visitor::visit(const FuncDeclaration* o) {
  o->param->accept(this);
}

void bi::Visitor::visit(const ModelDeclaration* o) {
  o->param->accept(this);
}

void bi::Visitor::visit(const ProgDeclaration* o) {
  o->param->accept(this);
}

void bi::Visitor::visit(const AssignableType* o) {
  o->single->accept(this);
}

void bi::Visitor::visit(const BracketsType* o) {
  o->single->accept(this);
  o->brackets->accept(this);
}

void bi::Visitor::visit(const ParenthesesType* o) {
  o->single->accept(this);
}

void bi::Visitor::visit(const RandomType* o) {
  o->left->accept(this);
  o->right->accept(this);
}

void bi::Visitor::visit(const LambdaType* o) {
  o->parens->accept(this);
  o->result->accept(this);
}

void bi::Visitor::visit(const TypeList* o) {
  o->head->accept(this);
  o->tail->accept(this);
}

void bi::Visitor::visit(const VariantType* o) {
  //
}
