/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Unary.hpp"
#include "bi/primitive/unique_ptr.hpp"

namespace bi {
/**
 * Tuple type.
 *
 * @ingroup compiler_type
 */
class ParenthesesType: public Type, public TypeUnary {
public:
  /**
   * Constructor.
   *
   * @param single Type in parentheses.
   * @param loc Location.
   * @param assignable Is this type assignable?
   */
  ParenthesesType(Type* single, shared_ptr<Location> loc = nullptr,
      const bool assignable = false);

  /**
   * Destructor.
   */
  virtual ~ParenthesesType();

  virtual bool isBuiltin() const;
  virtual bool isStruct() const;
  virtual bool isClass() const;
  virtual bool isAlias() const;
  virtual bool isLambda() const;

  virtual Type* strip();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const BracketsType& o) const;
  virtual bool definitely(const EmptyType& o) const;
  virtual bool definitely(const LambdaType& o) const;
  virtual bool definitely(const List<Type>& o) const;
  virtual bool definitely(const TypeParameter& o) const;
  virtual bool definitely(const TypeReference& o) const;
  virtual bool definitely(const ParenthesesType& o) const;

  virtual bool dispatchPossibly(const Type& o) const;
  virtual bool possibly(const BracketsType& o) const;
  virtual bool possibly(const EmptyType& o) const;
  virtual bool possibly(const LambdaType& o) const;
  virtual bool possibly(const List<Type>& o) const;
  virtual bool possibly(const TypeParameter& o) const;
  virtual bool possibly(const TypeReference& o) const;
  virtual bool possibly(const ParenthesesType& o) const;
};
}
