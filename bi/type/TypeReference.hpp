/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/type/TypeParameter.hpp"
#include "bi/expression/EmptyExpression.hpp"
#include "bi/common/Parenthesised.hpp"
#include "bi/common/Reference.hpp"
#include "bi/type/EmptyType.hpp"

namespace bi {
/**
 * Reference to type.
 *
 * @ingroup compiler_type
 */
class TypeReference: public Type,
    public Named,
    public Parenthesised,
    public Reference<TypeParameter> {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param parens Parentheses.
   * @param loc Location.
   * @param assignable Is this type assignable?
   * @param target Target.
   */
  TypeReference(shared_ptr<Name> name, Expression* parens =
      new EmptyExpression(), shared_ptr<Location> loc = nullptr,
      const bool assignable = false, TypeParameter* target = nullptr);

  /**
   * Constructor.
   *
   * @param target Target.
   */
  TypeReference(TypeParameter* target);

  /**
   * Destructor.
   */
  virtual ~TypeReference();

  virtual bool isBuiltin() const;
  virtual bool isStruct() const;
  virtual bool isClass() const;
  virtual bool isAlias() const;

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  using Type::definitely;
  using Type::possibly;

  /**
   * Do any of the conversions for this type definitely match?
   */
  bool convertedDefinitely(const Type& o) const;

  /**
   * Do any of the conversions for this type possibly match?
   */
  bool convertedPossibly(const Type& o) const;

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
