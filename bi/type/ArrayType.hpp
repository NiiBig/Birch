/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Bracketed.hpp"
#include "bi/common/Unary.hpp"
#include "bi/primitive/unique_ptr.hpp"

namespace bi {
/**
 * Array type.
 *
 * @ingroup compiler_type
 */
class ArrayType: public Type, public Unary<Type>, public Bracketed {
public:
  /**
   * Constructor.
   *
   * @param single Type.
   * @param brackets Brackets.
   * @param loc Location.
   * @param assignable Is this type assignable?
   */
  ArrayType(Type* single, Expression* brackets, shared_ptr<Location> loc =
      nullptr, const bool assignable = false);

  /**
   * Constructor.
   *
   * @param single Type.
   * @param ndims Number of dimensions.
   * @param loc Location.
   * @param assignable Is this type assignable?
   */
  ArrayType(Type* single, const int ndims, shared_ptr<Location> loc =
      nullptr, const bool assignable = false);

  /**
   * Destructor.
   */
  virtual ~ArrayType();

  virtual int count() const;
  virtual bool isArray() const;

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  using Type::definitely;
  using Type::possibly;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const ArrayType& o) const;

  virtual bool dispatchPossibly(const Type& o) const;
  virtual bool possibly(const ArrayType& o) const;

  /**
   * Number of dimensions.
   */
  int ndims;
};
}
