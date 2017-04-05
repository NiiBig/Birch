/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Parenthesised.hpp"
#include "bi/common/Based.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Braced.hpp"
#include "bi/common/Scoped.hpp"

namespace bi {
/**
 * Type parameter.
 *
 * @ingroup compiler_type
 */
class ModelParameter: public Type,
    public Named,
    public Parenthesised,
    public Based,
    public Braced,
    public Scoped {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param parens Parentheses.
   * @param op Operator giving relation to base type.
   * @param base Base type.
   * @param braces Braces.
   * @param loc Location.
   * @param assignable Is this type writeable?
   */
  ModelParameter(shared_ptr<Name> name, Expression* parens,
      shared_ptr<Name> op, Type* base, Expression* braces,
      shared_ptr<Location> loc = nullptr, const bool assignable = false);

  /**
   * Destructor.
   */
  virtual ~ModelParameter();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  bool isBuiltin() const;
  bool isModel() const;
  bool isLess() const;
  bool isEqual() const;

  /**
   * Get the base type.
   */
  const ModelParameter* getBase() const;

  /**
   * Is this type equal to or less than @p o by inheritance?
   */
  bool canUpcast(const ModelParameter* o) const;

  /**
   * Is this type equal to or greater than @p o by inheritance?
   */
  bool canDowncast(const ModelParameter* o) const;

  using Type::definitely;
  using Type::possibly;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const ModelParameter& o) const;
  virtual bool definitely(const EmptyType& o) const;

  virtual bool dispatchPossibly(const Type& o) const;
  virtual bool possibly(const ModelParameter& o) const;
  virtual bool possibly(const EmptyType& o) const;
};
}
