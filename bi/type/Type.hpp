/**
 * @file
 */
#pragma once

#include "bi/common/Located.hpp"

namespace bi {
class Cloner;
class Modifier;
class Visitor;

class Argumented;

class AliasType;
class ArrayType;
class BasicType;
class BinaryType;
class ClassType;
class FiberType;
class EmptyType;
class FunctionType;
class IdentifierType;
template<class T> class Iterator;
class ListType;
class NilType;
class OptionalType;
class OverloadedType;
class ParenthesesType;

class Class;
class Alias;
class Basic;

/**
 * Type.
 *
 * @ingroup compiler_type
 */
class Type: public Located {
public:
  /**
   * Constructor.
   *
   * @param loc Location.
   * @param assignable Is this type assignable?
   */
  Type(Location* loc = nullptr, const bool assignable = false);

  /**
   * Destructor.
   */
  virtual ~Type() = 0;

  /**
   * Accept cloning visitor.
   *
   * @param v The visitor.
   *
   * @return Cloned (and potentially modified) type.
   */
  virtual Type* accept(Cloner* visitor) const = 0;

  /**
   * Accept modifying visitor.
   *
   * @param v The visitor.
   *
   * @return Modified type.
   */
  virtual Type* accept(Modifier* visitor) = 0;

  /**
   * Accept read-only visitor.
   *
   * @param v The visitor.
   */
  virtual void accept(Visitor* visitor) const = 0;

  /**
   * Is this an empty type?
   */
  virtual bool isEmpty() const;

  /**
   * Is this a built-in type?
   */
  virtual bool isBasic() const;

  /**
   * Is this a class type, or an alias of a class type?
   */
  virtual bool isClass() const;

  /**
   * Is this an type?
   */
  virtual bool isAlias() const;

  /**
   * Is this an array type?
   */
  virtual bool isArray() const;

  /**
   * Is this a function type?
   */
  virtual bool isFunction() const;

  /**
   * Is this a fiber type?
   */
  virtual bool isFiber() const;

  /**
   * Is this an optional type?
   */
  virtual bool isOptional() const;

  /**
   * Is this a binary operator type?
   */
  virtual bool isBinary() const;

  /**
   * Is this an overloaded type?
   */
  virtual bool isOverloaded() const;

  /**
   * Get the left operand of a binary, otherwise undefined.
   */
  virtual Type* getLeft() const;

  /**
   * Get the right operand of a binary type, otherwise undefined.
   */
  virtual Type* getRight() const;

  /**
   * Get the class associated with a class type, otherwise undefined.
   */
  virtual Class* getClass() const;

  /**
   * For an optional or fiber type, the type that is wrapped, otherwise
   * undefined.
   */
  virtual Type* unwrap() const;

  /**
   * Resolve a call.
   *
   * @param args Arguments.
   *
   * @return If this is an overloaded type, and the argument types match the
   * parameter types of the function, the return type of the function. In all
   * other cases throws an exception.
   */
  virtual FunctionType* resolve(Argumented* args);

  /**
   * Resolve a constructor call.
   *
   * @param args Argument types.
   */
  virtual void resolveConstructor(Type* args);

  /**
   * How many dimensions does this type have?
   */
  virtual int count() const;

  /**
   * Iterator to first element if this is a list, to one-past-the-last if
   * this is empty, otherwise to this.
   */
  virtual Iterator<Type> begin() const;

  /**
   * Iterator to one-past-the-last.
   */
  virtual Iterator<Type> end() const;

  /**
   * Is this type assignable?
   */
  bool assignable;

  /*
   * Double-dispatch partial order comparisons.
   */
  virtual bool definitely(const Type& o) const;
  virtual bool dispatchDefinitely(const Type& o) const = 0;
  virtual bool definitely(const AliasType& o) const;
  virtual bool definitely(const ArrayType& o) const;
  virtual bool definitely(const BasicType& o) const;
  virtual bool definitely(const BinaryType& o) const;
  virtual bool definitely(const ClassType& o) const;
  virtual bool definitely(const FiberType& o) const;
  virtual bool definitely(const EmptyType& o) const;
  virtual bool definitely(const FunctionType& o) const;
  virtual bool definitely(const IdentifierType& o) const;
  virtual bool definitely(const ListType& o) const;
  virtual bool definitely(const NilType& o) const;
  virtual bool definitely(const OptionalType& o) const;
  virtual bool definitely(const OverloadedType& o) const;
  virtual bool definitely(const ParenthesesType& o) const;

  virtual bool possibly(const Type& o) const;
  virtual bool dispatchPossibly(const Type& o) const = 0;
  virtual bool possibly(const AliasType& o) const;
  virtual bool possibly(const ArrayType& o) const;
  virtual bool possibly(const BasicType& o) const;
  virtual bool possibly(const BinaryType& o) const;
  virtual bool possibly(const ClassType& o) const;
  virtual bool possibly(const FiberType& o) const;
  virtual bool possibly(const EmptyType& o) const;
  virtual bool possibly(const FunctionType& o) const;
  virtual bool possibly(const IdentifierType& o) const;
  virtual bool possibly(const ListType& o) const;
  virtual bool possibly(const NilType& o) const;
  virtual bool possibly(const OptionalType& o) const;
  virtual bool possibly(const OverloadedType& o) const;
  virtual bool possibly(const ParenthesesType& o) const;

  /**
   * Are these two types the same?
   */
  virtual bool equals(const Type& o) const;
};
}
