/**
 * @file
 */
#pragma once

#include "libbirch/Counted.hpp"
#include "libbirch/Map.hpp"

namespace bi {
template<class T> class SharedCOW;

/**
 * Base for all class types.
 *
 * @ingroup libbirch
 */
class Any: public Counted {
public:
  /**
   * Constructor.
   */
  Any();

  /**
   * Copy constructor.
   */
  Any(const Any& o);

  /**
   * Destructor.
   */
  virtual ~Any();

  /**
   * Clone the object.
   */
  virtual Any* clone() const;

  /**
   * Deallocate the memory for the object.
   */
  virtual void destroy();

  /**
   * Get the memo associated with the clone or construction of this object.
   */
  Memo* getMemo();

  /**
   * Shallow retrieval of an object that may not yet have been cloned,
   * cloning it if necessary.
   *
   * @param memo Memo associated with the clone.
   *
   * @return The cloned object.
   */
  Any* get(Memo* memo);

  /**
   * Shallow retrieval of an object that may not yet have been cloned,
   * without cloning it. This can be used as an optimization for read-only
   * access to value types.
   *
   * @param memo Memo associated with the clone.
   *
   * @return The mapped object.
   */
  Any* pull(Memo* memo);

  /**
   * Deep retrieval of an object that may not yet have been cloned,
   * without cloning it. This can be used as an optimization for read-only
   * access to value types.
   *
   * @param memo Memo associated with the clone.
   *
   * @return The mapped object.
   */
  Any* deepPull(Memo* memo);

protected:
  /**
   * Memo associated with the clone or construction of this object.
   */
  SharedPtr<Memo> memo;

  /**
   * Clones produced from this object.
   */
  Map clones;
};
}
