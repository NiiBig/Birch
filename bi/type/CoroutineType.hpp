/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"

#include "bi/primitive/unique_ptr.hpp"

namespace bi {
/**
 * Lambda function type.
 *
 * @ingroup compiler_type
 */
class CoroutineType: public Type {
public:
  /**
   * Constructor.
   *
   * @param type Return type.
   * @param loc Location.
   * @param assignable Is this type assignable?
   */
  CoroutineType(Type* type, shared_ptr<Location> loc = nullptr,
      const bool assignable = false);

  /**
   * Destructor.
   */
  virtual ~CoroutineType();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual bool isCoroutine() const;

  using Type::definitely;
  using Type::possibly;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const CoroutineType& o) const;
  virtual bool definitely(const ParenthesesType& o) const;

  virtual bool dispatchPossibly(const Type& o) const;
  virtual bool possibly(const CoroutineType& o) const;
  virtual bool possibly(const ParenthesesType& o) const;

  /**
   * Return type.
   */
  unique_ptr<Type> type;
};
}
