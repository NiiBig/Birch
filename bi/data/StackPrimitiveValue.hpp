/**
 * @file
 */
#pragma once

#include "bi/data/StackGroup.hpp"
#include "bi/data/HeapPrimitiveValue.hpp"
#include "bi/data/NetCDFPrimitiveValue.hpp"

namespace bi {
/**
 * Value for StackGroup.
 *
 * @ingroup library
 *
 * @tparam Type Primitive type.
 */
template<class Type>
class PrimitiveValue<Type,StackGroup> {
public:
  typedef StackGroup group_type;

  /**
   * Constructor.
   */
  PrimitiveValue(const EmptyFrame& frame = EmptyFrame(), const char* name =
      nullptr, const StackGroup& group = StackGroup());

  /**
   * Constructor from heap.
   */
  PrimitiveValue(const PrimitiveValue<Type,HeapGroup>& value);

  /**
   * Constructor from NetCDF file.
   */
  PrimitiveValue(const PrimitiveValue<Type,NetCDFGroup>& value);

  /**
   * Constructor from value.
   */
  explicit PrimitiveValue(const Type& value, const StackGroup& group =
      StackGroup());

  /**
   * View constructor.
   */
  PrimitiveValue(const PrimitiveValue<Type,StackGroup>& o,
      const EmptyFrame& frame, const EmptyView& view);

//  /**
//   * Shallow copy constructor.
//   */
//  PrimitiveValue(const PrimitiveValue<Type,StackGroup>& o);
//
//  /**
//   * Move constructor.
//   */
//  PrimitiveValue(PrimitiveValue<Type,StackGroup> && o);

  /**
   * Destructor.
   */
  ~PrimitiveValue();

  /**
   * Assignment.
   */
  PrimitiveValue<Type,StackGroup>& operator=(
      const PrimitiveValue<Type,StackGroup>& o);

  /**
   * Generic assignment.
   */
  template<class Type1, class Group1>
  PrimitiveValue<Type,StackGroup>& operator=(
      const PrimitiveValue<Type1,Group1>& o);

  /**
   * Value assignment.
   */
  PrimitiveValue<Type,StackGroup>& operator=(const Type& o);

  /**
   * Basic type conversion.
   */
  operator Type&();

  /**
   * Basic type conversion.
   */
  operator const Type&() const;

  /**
   * Underlying value.
   */
  Type value;

  /**
   * Group.
   */
  StackGroup group;
};
}

#include "bi/data/copy.hpp"

template<class Type>
template<class Type1, class Group1>
bi::PrimitiveValue<Type,bi::StackGroup>& bi::PrimitiveValue<Type,
    bi::StackGroup>::operator=(const PrimitiveValue<Type1,Group1>& o) {
  copy(*this, o);
  return *this;
}
