/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Single.hpp"

namespace bi {
/**
 * Optional type.
 *
 * @ingroup compiler_type
 */
class OptionalType: public Type, public Single<Type> {
public:
  /**
   * Constructor.
   *
   * @param single Type.
   * @param loc Location.
   * @param assignable Is this type assignable?
   */
  OptionalType(Type* single, Location* loc = nullptr, const bool assignable =
      false);

  /**
   * Destructor.
   */
  virtual ~OptionalType();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  virtual bool isOptional() const;
  virtual Type* unwrap() const;

  using Type::definitely;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const AliasType& o) const;
  virtual bool definitely(const GenericType& o) const;
  virtual bool definitely(const OptionalType& o) const;
};
}
