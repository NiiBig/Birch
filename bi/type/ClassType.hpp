/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Reference.hpp"
#include "bi/statement/Class.hpp"

namespace bi {
/**
 * Class type.
 *
 * @ingroup compiler_type
 */
class ClassType: public Type, public Named, public Reference<Class> {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param loc Location.
   * @param assignable Is this type assignable?
   * @param target Target.
   */
  ClassType(shared_ptr<Name> name, shared_ptr<Location> loc = nullptr,
      const bool assignable = false, const Class* target = nullptr);

  /**
   * Constructor.
   *
   * @param target Target.
   */
  ClassType(const Class* target);

  /**
   * Destructor.
   */
  virtual ~ClassType();

  virtual bool isClass() const;

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  using Type::definitely;
  using Type::possibly;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const AliasType& o) const;
  virtual bool definitely(const ArrayType& o) const;
  virtual bool definitely(const BasicType& o) const;
  virtual bool definitely(const ClassType& o) const;
  virtual bool definitely(const CoroutineType& o) const;
  virtual bool definitely(const FunctionType& o) const;
  virtual bool definitely(const List<Type>& o) const;
  virtual bool definitely(const ParenthesesType& o) const;
  virtual bool definitely(const Class& o) const;

  virtual bool dispatchPossibly(const Type& o) const;
  virtual bool possibly(const AliasType& o) const;
  virtual bool possibly(const ClassType& o) const;
  virtual bool possibly(const ParenthesesType& o) const;
};
}
