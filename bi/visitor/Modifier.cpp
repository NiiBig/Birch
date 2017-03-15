/**
 * @file
 */
#include "bi/visitor/Modifier.hpp"

bi::Modifier::~Modifier() {
  //
}

bi::Expression* bi::Modifier::modify(EmptyExpression* o) {
  return o;
}

bi::Statement* bi::Modifier::modify(EmptyStatement* o) {
  return o;
}

bi::Type* bi::Modifier::modify(EmptyType* o) {
  return o;
}

bi::Expression* bi::Modifier::modify(BooleanLiteral* o) {
  o->type = o->type->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(IntegerLiteral* o) {
  o->type = o->type->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(RealLiteral* o) {
  o->type = o->type->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(StringLiteral* o) {
  o->type = o->type->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(ExpressionList* o) {
  o->head = o->head->accept(this);
  o->tail = o->tail->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(StatementList* o) {
  o->head = o->head->accept(this);
  o->tail = o->tail->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(ParenthesesExpression* o) {
  o->single = o->single->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(BracesExpression* o) {
  o->single = o->single->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(BracketsExpression* o) {
  o->single = o->single->accept(this);
  o->brackets = o->brackets->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(Index* o) {
  o->single = o->single->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(Range* o) {
  o->left = o->left->accept(this);
  o->right = o->right->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(Member* o) {
  o->left = o->left->accept(this);
  o->right = o->right->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(This* o) {
  return o;
}

bi::Expression* bi::Modifier::modify(LambdaInit* o) {
  o->single = o->single->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(RandomInit* o) {
  o->left = o->left->accept(this);
  o->right = o->right->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(VarReference* o) {
  return o;
}

bi::Expression* bi::Modifier::modify(FuncReference* o) {
  o->parens = o->parens->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(ModelReference* o) {
  o->parens = o->parens->accept(this);
  return o;
}

bi::Prog* bi::Modifier::modify(ProgReference* o) {
  o->parens = o->parens->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(VarParameter* o) {
  o->type = o->type->accept(this);
  o->value = o->value->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(FuncParameter* o) {
  o->parens = o->parens->accept(this);
  o->result = o->result->accept(this);
  o->braces = o->braces->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(ModelParameter* o) {
  o->parens = o->parens->accept(this);
  o->base = o->base->accept(this);
  o->braces = o->braces->accept(this);
  return o;
}

bi::Prog* bi::Modifier::modify(ProgParameter* o) {
  o->parens = o->parens->accept(this);
  o->braces = o->braces->accept(this);
  return o;
}

void bi::Modifier::modify(File* o) {
  o->root = o->root->accept(this);
}

bi::Statement* bi::Modifier::modify(Import* o) {
  return o;
}

bi::Statement* bi::Modifier::modify(ExpressionStatement* o) {
  o->single = o->single->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(Conditional* o) {
  o->cond = o->cond->accept(this);
  o->braces = o->braces->accept(this);
  o->falseBraces = o->falseBraces->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(Loop* o) {
  o->cond = o->cond->accept(this);
  o->braces = o->braces->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(Raw* o) {
  return o;
}

bi::Statement* bi::Modifier::modify(VarDeclaration* o) {
  o->param = dynamic_cast<VarParameter*>(o->param->accept(this));
  assert(o->param);
  return o;
}

bi::Statement* bi::Modifier::modify(FuncDeclaration* o) {
  o->param = dynamic_cast<FuncParameter*>(o->param->accept(this));
  assert(o->param);
  return o;
}

bi::Statement* bi::Modifier::modify(ModelDeclaration* o) {
  o->param = dynamic_cast<ModelParameter*>(o->param->accept(this));
  assert(o->param);
  return o;
}

bi::Statement* bi::Modifier::modify(ProgDeclaration* o) {
  o->param = dynamic_cast<ProgParameter*>(o->param->accept(this));
  assert(o->param);
  return o;
}

bi::Type* bi::Modifier::modify(AssignableType* o) {
  o->single = o->single->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(BracketsType* o) {
  o->single = o->single->accept(this);
  o->brackets = o->brackets->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(ParenthesesType* o) {
  o->single = o->single->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(RandomType* o) {
  o->left = o->left->accept(this);
  o->right = o->right->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(LambdaType* o) {
  o->result = o->result->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(TypeList* o) {
  o->head = o->head->accept(this);
  o->tail = o->tail->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(VariantType* o) {
  for (auto iter = o->types.begin(); iter != o->types.end(); ++iter) {
    *iter = (*iter)->accept(this);
  }
  return o;
}

bi::Dispatcher* bi::Modifier::modify(Dispatcher* o) {
  return o;
}
