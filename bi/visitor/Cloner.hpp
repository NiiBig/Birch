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
 * @ingroup visitor
 */
class Cloner {
public:
  /**
   * Destructor.
   */
  virtual ~Cloner();

  virtual Package* clone(const Package* o);
  virtual File* clone(const File* o);

  virtual Expression* clone(const EmptyExpression* o);
  virtual Expression* clone(const ExpressionList* o);
  virtual Expression* clone(const Literal<bool>* o);
  virtual Expression* clone(const Literal<int64_t>* o);
  virtual Expression* clone(const Literal<double>* o);
  virtual Expression* clone(const Literal<const char*>* o);
  virtual Expression* clone(const Parentheses* o);
  virtual Expression* clone(const Sequence* o);
  virtual Expression* clone(const Binary* o);
  virtual Expression* clone(const Cast* o);
  virtual Expression* clone(const Call<Unknown>* o);
  virtual Expression* clone(const Call<Function>* o);
  virtual Expression* clone(const Call<MemberFunction>* o);
  virtual Expression* clone(const Call<Fiber>* o);
  virtual Expression* clone(const Call<MemberFiber>* o);
  virtual Expression* clone(const Call<Parameter>* o);
  virtual Expression* clone(const Call<FiberParameter>* o);
  virtual Expression* clone(const Call<LocalVariable>* o);
  virtual Expression* clone(const Call<FiberVariable>* o);
  virtual Expression* clone(const Call<MemberVariable>* o);
  virtual Expression* clone(const Call<GlobalVariable>* o);
  virtual Expression* clone(const Call<BinaryOperator>* o);
  virtual Expression* clone(const Call<UnaryOperator>* o);
  virtual Expression* clone(const Assign* o);
  virtual Expression* clone(const Slice* o);
  virtual Expression* clone(const Query* o);
  virtual Expression* clone(const Get* o);
  virtual Expression* clone(const LambdaFunction* o);
  virtual Expression* clone(const Span* o);
  virtual Expression* clone(const Index* o);
  virtual Expression* clone(const Range* o);
  virtual Expression* clone(const Member* o);
  virtual Expression* clone(const Global* o);
  virtual Expression* clone(const Super* o);
  virtual Expression* clone(const This* o);
  virtual Expression* clone(const Nil* o);
  virtual Expression* clone(const Parameter* o);
  virtual Expression* clone(const FiberParameter* o);
  virtual Expression* clone(const Generic* o);
  virtual Expression* clone(const Identifier<Unknown>* o);
  virtual Expression* clone(const Identifier<Parameter>* o);
  virtual Expression* clone(const Identifier<FiberParameter>* o);
  virtual Expression* clone(const Identifier<GlobalVariable>* o);
  virtual Expression* clone(const Identifier<MemberVariable>* o);
  virtual Expression* clone(const Identifier<FiberVariable>* o);
  virtual Expression* clone(const Identifier<LocalVariable>* o);
  virtual Expression* clone(const Identifier<ForVariable>* o);
  virtual Expression* clone(const Identifier<ParallelVariable>* o);
  virtual Expression* clone(const OverloadedIdentifier<Unknown>* o);
  virtual Expression* clone(const OverloadedIdentifier<Function>* o);
  virtual Expression* clone(const OverloadedIdentifier<Fiber>* o);
  virtual Expression* clone(const OverloadedIdentifier<MemberFunction>* o);
  virtual Expression* clone(const OverloadedIdentifier<MemberFiber>* o);
  virtual Expression* clone(const OverloadedIdentifier<BinaryOperator>* o);
  virtual Expression* clone(const OverloadedIdentifier<UnaryOperator>* o);

  virtual Statement* clone(const EmptyStatement* o);
  virtual Statement* clone(const Braces* o);
  virtual Statement* clone(const StatementList* o);
  virtual Statement* clone(const Assume* o);
  virtual Statement* clone(const GlobalVariable* o);
  virtual Statement* clone(const MemberVariable* o);
  virtual Statement* clone(const FiberVariable* o);
  virtual Statement* clone(const LocalVariable* o);
  virtual Statement* clone(const ForVariable* o);
  virtual Statement* clone(const ParallelVariable* o);
  virtual Statement* clone(const Function* o);
  virtual Statement* clone(const Fiber* o);
  virtual Statement* clone(const Program* o);
  virtual Statement* clone(const MemberFunction* o);
  virtual Statement* clone(const MemberFiber* o);
  virtual Statement* clone(const BinaryOperator* o);
  virtual Statement* clone(const UnaryOperator* o);
  virtual Statement* clone(const AssignmentOperator* o);
  virtual Statement* clone(const ConversionOperator* o);
  virtual Statement* clone(const Class* o);
  virtual Statement* clone(const Basic* o);
  virtual Statement* clone(const ExpressionStatement* o);
  virtual Statement* clone(const If* o);
  virtual Statement* clone(const For* o);
  virtual Statement* clone(const Parallel* o);
  virtual Statement* clone(const While* o);
  virtual Statement* clone(const DoWhile* o);
  virtual Statement* clone(const Assert* o);
  virtual Statement* clone(const Return* o);
  virtual Statement* clone(const Yield* o);
  virtual Statement* clone(const Raw* o);
  virtual Statement* clone(const Instantiated<Type>* o);
  virtual Statement* clone(const Instantiated<Expression>* o);

  virtual Type* clone(const EmptyType* o);
  virtual Type* clone(const TypeList* o);
  virtual Type* clone(const UnknownType* o);
  virtual Type* clone(const ClassType* o);
  virtual Type* clone(const BasicType* o);
  virtual Type* clone(const GenericType* o);
  virtual Type* clone(const MemberType* o);
  virtual Type* clone(const ArrayType* o);
  virtual Type* clone(const TupleType* o);
  virtual Type* clone(const BinaryType* o);
  virtual Type* clone(const FunctionType* o);
  virtual Type* clone(const FiberType* o);
  virtual Type* clone(const OptionalType* o);
  virtual Type* clone(const NilType* o);
};
}
