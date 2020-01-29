/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/TypeArgumented.hpp"
#include "bi/common/Reference.hpp"
#include "bi/statement/Class.hpp"

namespace bi {
/**
 * Should conversions be considered when comparing types?
 *
 * @internal This global variable is part of a hack used by Resolver to
 * disambiguate overload resolution in some circumstances, favouring
 * resolutions that do not require implicit type conversion.
 */
extern bool allowConversions;

/**
 * Class type.
 *
 * @ingroup type
 */
class ClassType: public Type,
    public Named,
    public TypeArgumented,
    public Reference<Class> {
public:
  /**
   * Constructor.
   *
   * @param weak Is this annotated weak?
   * @param name Name.
   * @param typeArgs Generic type arguments.
   * @param loc Location.
   * @param target Target.
   */
  ClassType(const bool weak, Name* name, Type* typeArgs,
      Location* loc = nullptr, Class* target = nullptr);

  /**
   * Constructor.
   *
   * @param target Target.
   * @param loc Location.
   */
  ClassType(Class* target, Location* loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~ClassType();

  virtual bool isClass() const;
  virtual Class* getClass() const;

  virtual Type* canonical();
  virtual const Type* canonical() const;

  virtual void resolveConstructor(Argumented* args);

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  /**
   * Should this use a weak pointer?
   */
  bool weak;

  using Type::isConvertible;
  using Type::isAssignable;

  virtual bool dispatchIsConvertible(const Type& o) const;
  virtual bool isConvertible(const GenericType& o) const;
  virtual bool isConvertible(const MemberType& o) const;
  virtual bool isConvertible(const ArrayType& o) const;
  virtual bool isConvertible(const BasicType& o) const;
  virtual bool isConvertible(const ClassType& o) const;
  virtual bool isConvertible(const FiberType& o) const;
  virtual bool isConvertible(const FunctionType& o) const;
  virtual bool isConvertible(const OptionalType& o) const;
  virtual bool isConvertible(const TupleType& o) const;

  virtual bool dispatchIsAssignable(const Type& o) const;
  virtual bool isAssignable(const GenericType& o) const;
  virtual bool isAssignable(const MemberType& o) const;
  virtual bool isAssignable(const ArrayType& o) const;
  virtual bool isAssignable(const BasicType& o) const;
  virtual bool isAssignable(const ClassType& o) const;
  virtual bool isAssignable(const FiberType& o) const;
  virtual bool isAssignable(const FunctionType& o) const;
  virtual bool isAssignable(const OptionalType& o) const;
  virtual bool isAssignable(const TupleType& o) const;
};
}
