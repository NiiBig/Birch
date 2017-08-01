/**
 * @file
 */
#pragma once

#include "bi/type/Type.hpp"
#include "bi/common/Iterator.hpp"

namespace bi {
/**
 * List type.
 *
 * @ingroup compiler_common
 */
class ListType: public Type {
public:
  /**
   * Constructor.
   *
   * @param head First in list.
   * @param tail Remaining list.
   * @param loc Location.
   */
  ListType(Type* head, Type* tail, shared_ptr<Location> loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~ListType();

  virtual Type* accept(Cloner* visitor) const;
  virtual Type* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  /**
   * Left operand.
   */
  unique_ptr<Type> head;

  /**
   * Right operand.
   */
  unique_ptr<Type> tail;

  virtual bool dispatchDefinitely(const Type& o) const;
  virtual bool definitely(const ListType& o) const;

  virtual bool dispatchPossibly(const Type& o) const;
  virtual bool possibly(const ListType& o) const;
};
}
