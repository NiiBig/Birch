/**
 * @file
 */
#pragma once

#include "bi/io/indentable_ostream.hpp"

namespace bi {
/**
 * Output stream for Birch source files.
 *
 * @ingroup io
 */
class bi_ostream: public indentable_ostream {
public:
  bi_ostream(std::ostream& base, const int level = 0, const bool header =
      false);

  using indentable_ostream::visit;

  virtual void visit(const Package* o);
  virtual void visit(const Name* o);

  virtual void visit(const ExpressionList* o);
  virtual void visit(const Literal<bool>* o);
  virtual void visit(const Literal<int64_t>* o);
  virtual void visit(const Literal<double>* o);
  virtual void visit(const Literal<const char*>* o);
  virtual void visit(const Parentheses* o);
  virtual void visit(const Sequence* o);
  virtual void visit(const Cast* o);
  virtual void visit(const Call<Unknown>* o);
  virtual void visit(const Call<Function>* o);
  virtual void visit(const Call<MemberFunction>* o);
  virtual void visit(const Call<Fiber>* o);
  virtual void visit(const Call<MemberFiber>* o);
  virtual void visit(const Call<Parameter>* o);
  virtual void visit(const Call<LocalVariable>* o);
  virtual void visit(const Call<MemberVariable>* o);
  virtual void visit(const Call<GlobalVariable>* o);
  virtual void visit(const Call<BinaryOperator>* o);
  virtual void visit(const Call<UnaryOperator>* o);
  virtual void visit(const Assign* o);
  virtual void visit(const Slice* o);
  virtual void visit(const Query* o);
  virtual void visit(const Get* o);
  virtual void visit(const LambdaFunction* o);
  virtual void visit(const Span* o);
  virtual void visit(const Index* o);
  virtual void visit(const Range* o);
  virtual void visit(const Member* o);
  virtual void visit(const Global* o);
  virtual void visit(const Super* o);
  virtual void visit(const This* o);
  virtual void visit(const Nil* o);
  virtual void visit(const Parameter* o);
  virtual void visit(const Generic* o);
  virtual void visit(const GlobalVariable* o);
  virtual void visit(const MemberVariable* o);
  virtual void visit(const LocalVariable* o);
  virtual void visit(const ForVariable* o);
  virtual void visit(const ParallelVariable* o);
  virtual void visit(const Identifier<Parameter>* o);
  virtual void visit(const Identifier<GlobalVariable>* o);
  virtual void visit(const Identifier<MemberVariable>* o);
  virtual void visit(const Identifier<LocalVariable>* o);
  virtual void visit(const Identifier<ForVariable>* o);
  virtual void visit(const Identifier<ParallelVariable>* o);
  virtual void visit(const Identifier<Unknown>* o);
  virtual void visit(const OverloadedIdentifier<Function>* o);
  virtual void visit(const OverloadedIdentifier<Fiber>* o);
  virtual void visit(const OverloadedIdentifier<MemberFunction>* o);
  virtual void visit(const OverloadedIdentifier<MemberFiber>* o);
  virtual void visit(const OverloadedIdentifier<BinaryOperator>* o);
  virtual void visit(const OverloadedIdentifier<UnaryOperator>* o);
  virtual void visit(const OverloadedIdentifier<Unknown>* o);

  virtual void visit(const Braces* o);
  virtual void visit(const Assume* o);
  virtual void visit(const Function* o);
  virtual void visit(const Fiber* o);
  virtual void visit(const Program* o);
  virtual void visit(const MemberFunction* o);
  virtual void visit(const MemberFiber* o);
  virtual void visit(const BinaryOperator* o);
  virtual void visit(const UnaryOperator* o);
  virtual void visit(const AssignmentOperator* o);
  virtual void visit(const ConversionOperator* o);
  virtual void visit(const Class* o);
  virtual void visit(const Basic* o);
  virtual void visit(const ExpressionStatement* o);
  virtual void visit(const If* o);
  virtual void visit(const For* o);
  virtual void visit(const Parallel* o);
  virtual void visit(const While* o);
  virtual void visit(const DoWhile* o);
  virtual void visit(const Assert* o);
  virtual void visit(const Return* o);
  virtual void visit(const Yield* o);
  virtual void visit(const Raw* o);
  virtual void visit(const StatementList* o);

  virtual void visit(const ClassType* o);
  virtual void visit(const BasicType* o);
  virtual void visit(const GenericType* o);
  virtual void visit(const MemberType* o);
  virtual void visit(const BinaryType* o);
  virtual void visit(const ArrayType* o);
  virtual void visit(const TupleType* o);
  virtual void visit(const FunctionType* o);
  virtual void visit(const FiberType* o);
  virtual void visit(const OptionalType* o);
  virtual void visit(const UnknownType* o);
  virtual void visit(const TypeList* o);

private:
  /**
   * The current class.
   */
  const Class* type;
};
}
