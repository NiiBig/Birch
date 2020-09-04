/**
 * @file
 */
#pragma once

#include "src/type/Type.hpp"
#include "src/common/ReturnTyped.hpp"

namespace birch {
/**
 * Function type.
 *
 * @ingroup type
 */
class FunctionType: public Type, public ReturnTyped {
public:
  /**
   * Constructor.
   *
   * @param params Parameters type.
   * @param returnType Return type.
   * @param loc Location.
   */
  FunctionType(Type* params, Type* returnType, Location* loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~FunctionType();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual bool isFunction() const;
  virtual bool isValue() const;

  /**
   * Parameters type.
   */
  Type* params;
};
}
