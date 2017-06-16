/**
 * @file
 */
#pragma once

#include "bi/expression/all.hpp"
#include "bi/statement/all.hpp"
#include "bi/type/all.hpp"

namespace bi {
/**
 * Cloning visitor.
 *
 * @ingroup compiler_visitor
 */
class Cloner {
public:
  /**
   * Destructor.
   */
  virtual ~Cloner();

  virtual File* clone(const File* o);

  virtual Expression* clone(const EmptyExpression* o);
  virtual Expression* clone(const List<Expression>* o);
  virtual Expression* clone(const Literal<bool>* o);
  virtual Expression* clone(const Literal<int64_t>* o);
  virtual Expression* clone(const Literal<double>* o);
  virtual Expression* clone(const Literal<const char*>* o);
  virtual Expression* clone(const ParenthesesExpression* o);
  virtual Expression* clone(const BracesExpression* o);
  virtual Expression* clone(const BracketsExpression* o);
  virtual Expression* clone(const LambdaFunction* o);
  virtual Expression* clone(const Span* o);
  virtual Expression* clone(const Index* o);
  virtual Expression* clone(const Range* o);
  virtual Expression* clone(const Member* o);
  virtual Expression* clone(const Super* o);
  virtual Expression* clone(const This* o);
  virtual Expression* clone(const Parameter* o);
  virtual Expression* clone(const GlobalVariable* o);
  virtual Expression* clone(const LocalVariable* o);
  virtual Expression* clone(const MemberVariable* o);
  virtual Expression* clone(const Identifier<Unknown>* o);
  virtual Expression* clone(const Identifier<Parameter>* o);
  virtual Expression* clone(const Identifier<GlobalVariable>* o);
  virtual Expression* clone(const Identifier<LocalVariable>* o);
  virtual Expression* clone(const Identifier<MemberVariable>* o);
  virtual Expression* clone(const Identifier<Function>* o);
  virtual Expression* clone(const Identifier<Coroutine>* o);
  virtual Expression* clone(const Identifier<MemberFunction>* o);
  virtual Expression* clone(const Identifier<BinaryOperator>* o);
  virtual Expression* clone(const Identifier<UnaryOperator>* o);

  virtual Statement* clone(const EmptyStatement* o);
  virtual Statement* clone(const List<Statement>* o);
  virtual Statement* clone(const Assignment* o);
  virtual Statement* clone(const Function* o);
  virtual Statement* clone(const Coroutine* o);
  virtual Statement* clone(const Program* o);
  virtual Statement* clone(const MemberFunction* o);
  virtual Statement* clone(const BinaryOperator* o);
  virtual Statement* clone(const UnaryOperator* o);
  virtual Statement* clone(const AssignmentOperator* o);
  virtual Statement* clone(const ConversionOperator* o);
  virtual Statement* clone(const Import* o);
  virtual Statement* clone(const ExpressionStatement* o);
  virtual Statement* clone(const If* o);
  virtual Statement* clone(const For* o);
  virtual Statement* clone(const While* o);
  virtual Statement* clone(const Return* o);
  virtual Statement* clone(const Raw* o);

  virtual Type* clone(const EmptyType* o);
  virtual Type* clone(const List<Type>* o);
  virtual Type* clone(const TypeReference* o);
  virtual Type* clone(const TypeParameter* o);
  virtual Type* clone(const BracketsType* o);
  virtual Type* clone(const ParenthesesType* o);
  virtual Type* clone(const FunctionType* o);
  virtual Type* clone(const CoroutineType* o);
};
}
