/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/ReturnTyped.hpp"

namespace bi {
/**
 * Function type.
 *
 * @ingroup compiler_type
 */
class FunctionType: public Type, public ReturnTyped {
public:
  /**
   * Constructor.
   *
   * @param parens Parameters type.
   * @param returnType Return type.
   * @param loc Location.
   * @param assignable Is this type assignable?
   */
  FunctionType(Type* parens, Type* returnType = new EmptyType(),
      shared_ptr<Location> loc = nullptr, const bool assignable = false);

  /**
   * Destructor.
   */
  virtual ~FunctionType();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual bool isFunction() const;

  using Type::definitely;
  using Type::possibly;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const FunctionType& o) const;
  virtual bool definitely(const ParenthesesType& o) const;

  virtual bool dispatchPossibly(const Type& o) const;
  virtual bool possibly(const FunctionType& o) const;
  virtual bool possibly(const ParenthesesType& o) const;

  /**
   * Parameters type.
   */
  unique_ptr<Type> parens;
};
}