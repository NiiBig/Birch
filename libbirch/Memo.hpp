/**
 * @file
 */
#pragma once

#include "libbirch/config.hpp"
#include "libbirch/Counted.hpp"

namespace bi {
/**
 * Memo for lazy deep cloning of objects.
 *
 * @ingroup libbirch
 */
class Memo: public Counted {
public:
  /**
   * Constructor.
   *
   * @param parent Parent.
   */
  Memo(Memo* parent = nullptr);

  /**
   * Destructor.
   */
  virtual ~Memo();

  /**
   * Deallocate.
   */
  virtual void destroy();

  /**
   * Is the given memo an ancestor of this?
   */
  bool hasAncestor(Memo* memo) const;

  /**
   * Get the parent of this memo.
   */
  Memo* getParent();

private:
  /**
   * Parent memo.
   */
  Memo* parent;
};
}
