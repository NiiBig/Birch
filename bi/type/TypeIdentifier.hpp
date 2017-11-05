/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Named.hpp"

namespace bi {
/**
 * Unresolved identifier for a type. This is used as a placeholder by the
 * parser for identifiers used in a type context, before they have been
 * resolved to an actual type.
 *
 * @ingroup compiler_type
 */
class TypeIdentifier: public Type, public Named {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param loc Location.
   */
  TypeIdentifier(Name* name, Location* loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~TypeIdentifier();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  using Type::definitely;
  using Type::common;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual Type* dispatchCommon(const Type& o) const;
};
}
