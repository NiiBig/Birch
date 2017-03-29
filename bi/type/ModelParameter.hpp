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

  using Type::definitely;
  using Type::possibly;

  virtual bool dispatchDefinitely(Type& o);
  virtual bool definitely(ModelParameter& o);
  virtual bool definitely(EmptyType& o);

  virtual bool dispatchPossibly(Type& o);
  virtual bool possibly(ModelParameter& o);
  virtual bool possibly(EmptyType& o);
};
}
