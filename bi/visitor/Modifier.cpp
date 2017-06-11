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
  o->type = o->type.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(IntegerLiteral* o) {
  o->type = o->type.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(RealLiteral* o) {
  o->type = o->type.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(StringLiteral* o) {
  o->type = o->type.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(List<Expression>* o) {
  o->head = o->head.release()->accept(this);
  o->tail = o->tail.release()->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(List<Statement>* o) {
  o->head = o->head.release()->accept(this);
  o->tail = o->tail.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(ParenthesesExpression* o) {
  o->single = o->single.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(BracesExpression* o) {
  o->single = o->single.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(BracketsExpression* o) {
  o->single = o->single.release()->accept(this);
  o->brackets = o->brackets.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(Span* o) {
  o->single = o->single.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(Index* o) {
  o->single = o->single.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(Range* o) {
  o->left = o->left.release()->accept(this);
  o->right = o->right.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(Member* o) {
  o->left = o->left.release()->accept(this);
  o->right = o->right.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(Super* o) {
  return o;
}

bi::Expression* bi::Modifier::modify(This* o) {
  return o;
}

bi::Expression* bi::Modifier::modify(VarReference* o) {
  return o;
}

bi::Expression* bi::Modifier::modify(FuncReference* o) {
  o->parens = o->parens.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(BinaryReference* o) {
  o->left = o->left.release()->accept(this);
  o->right = o->right.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(UnaryReference* o) {
  o->single = o->single.release()->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(TypeReference* o) {
  return o;
}

bi::Expression* bi::Modifier::modify(ProgReference* o) {
  o->parens = o->parens.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(VarParameter* o) {
  o->type = o->type.release()->accept(this);
  o->parens = o->parens.release()->accept(this);
  o->value = o->value.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(FuncParameter* o) {
  o->parens = o->parens.release()->accept(this);
  o->type = o->type.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(BinaryParameter* o) {
  o->left = o->left.release()->accept(this);
  o->right = o->right.release()->accept(this);
  o->type = o->type.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(UnaryParameter* o) {
  o->single = o->single.release()->accept(this);
  o->type = o->type.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(ConversionParameter* o) {
  o->type = o->type.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(TypeParameter* o) {
  o->parens = o->parens.release()->accept(this);
  o->base = o->base.release()->accept(this);
  o->baseParens = o->baseParens.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  return o;
}

bi::Expression* bi::Modifier::modify(ProgParameter* o) {
  o->parens = o->parens.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  return o;
}

void bi::Modifier::modify(File* o) {
  o->root = o->root.release()->accept(this);
}

bi::Statement* bi::Modifier::modify(Import* o) {
  return o;
}

bi::Statement* bi::Modifier::modify(ExpressionStatement* o) {
  o->single = o->single.release()->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(If* o) {
  o->cond = o->cond.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  o->falseBraces = o->falseBraces.release()->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(For* o) {
  o->index = o->index.release()->accept(this);
  o->from = o->from.release()->accept(this);
  o->to = o->to.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(While* o) {
  o->cond = o->cond.release()->accept(this);
  o->braces = o->braces.release()->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(Return* o) {
  o->single = o->single.release()->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(Raw* o) {
  return o;
}

bi::Statement* bi::Modifier::modify(Declaration<Expression>* o) {
  o->param = o->param.release()->accept(this);
  return o;
}

bi::Statement* bi::Modifier::modify(Declaration<Type>* o) {
  o->param = o->param.release()->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(BracketsType* o) {
  o->single = o->single.release()->accept(this);
  o->brackets = o->brackets.release()->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(ParenthesesType* o) {
  o->single = o->single.release()->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(FunctionType* o) {
  o->parens = o->parens.release()->accept(this);
  o->type = o->type.release()->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(CoroutineType* o) {
  o->type = o->type.release()->accept(this);
  return o;
}

bi::Type* bi::Modifier::modify(List<Type>* o) {
  o->head = o->head.release()->accept(this);
  o->tail = o->tail.release()->accept(this);
  return o;
}
