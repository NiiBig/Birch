/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"

namespace bi {
/**
 * Empty type.
 *
 * @ingroup compiler_type
 */
class EmptyType: public Type {
public:
  /**
   * Constructor.
   *
   * @param assignable Is this type assignable?
   * @param polymorphic Is this type polymorphic?
   */
  EmptyType(const bool assignable = false, const bool polymorphic = false);

  /**
   * Destructor.
   */
  virtual ~EmptyType();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual bool isEmpty() const;

  using Type::definitely;
  using Type::possibly;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const EmptyType& o) const;

  virtual bool dispatchPossibly(const Type& o) const;
  virtual bool possibly(const EmptyType& o) const;
};
}
