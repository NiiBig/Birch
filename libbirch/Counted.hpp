/**
 * @file
 */
#pragma once

#include "libbirch/config.hpp"

namespace bi {
/**
 * Base class for reference counted objects.
 *
 * @attention In order to work correctly, Counted must be the *first* base
 * class in any inheritance hierarchy. This is particularly important when
 * multiple inheritance is used.
 *
 * @ingroup libbirch
 */
class Counted {
protected:
  /**
   * Constructor.
   */
  Counted();

  /**
   * Copy constructor.
   */
  Counted(const Counted& o);

  /**
   * Destructor.
   */
  virtual ~Counted();

  /**
   * Assignment operator.
   */
  Counted& operator=(const Counted&) = delete;

public:
  /**
   * Create an object,
   */
  template<class... Args>
  static Counted* create(Args... args) {
    return emplace(allocate<sizeof(Counted)>(), args...);
  }

  /**
   * Create an object in previously-allocated memory.
   */
  template<class... Args>
  static Counted* emplace(void* ptr, Args... args) {
    auto o = new (ptr) Counted();
    o->size = sizeof(Counted);
    return o;
  }

  /**
   * Clone the object.
   */
  virtual Counted* clone() const {
    return emplace(allocate<sizeof(Counted)>(), *this);
  }

  /**
   * Clone the object into previous allocation.
   */
  virtual Counted* clone(void* ptr) const {
    return emplace(ptr, *this);
  }

  /**
   * Destroy the object.
   */
  virtual void destroy() {
    this->~Counted();
  }

  /**
   * Deallocate the object.
   */
  void deallocate();

  /**
   * Get the size, in bytes, of the object.
   */
  unsigned getSize() const;

  /**
   * If the object has yet to be destroyed, increment the shared count
   * and return a pointer to this. Otherwise return null. This is used
   * to atomically convert a WeakPtr into a SharedPtr.
   */
  Counted* lock();

  /**
   * Increment the shared count.
   */
  void incShared();

  /**
   * Decrement the shared count.
   */
  void decShared();

  /**
   * Shared count.
   */
  unsigned numShared() const;

  /**
   * Increment the weak count.
   */
  void incWeak();

  /**
   * Decrement the weak count.
   */
  void decWeak();

  /**
   * Weak count.
   */
  unsigned numWeak() const;

  /**
   * Increment the memo count (implies an increment of the weak count also).
   */
  void incMemo();

  /**
   * Decrement the memo count (implies a decrement of the weak count also).
   */
  void decMemo();

  /**
   * Is the object reachable? An object is reachable if it contains a shared
   * count of one or more, or a weak count greater than the memo count. When
   * the weak count equals the memo count (it cannot be less), the object
   * is only reachable via keys in memos, which will never be triggered, and
   * so the object is not considered reachable.
   */
  bool isReachable() const;

  /**
   * Is the object frozen?
   */
  bool isFrozen() const;

  /**
   * Freeze this object.
   */
  void freeze();

protected:
  /**
   * Perform the actual freeze of the object. This is overwritten by derived
   * classes. The non-virtual freeze() handles thread safety so that this
   * need not.
   */
  virtual void doFreeze();

  /**
   * Shared count.
   */
  std::atomic<unsigned> sharedCount;

  /**
   * Weak count.
   */
  std::atomic<unsigned> weakCount;

  /**
   * Memo count. This is the number of times that the object occurs as a key
   * in a memo. It is always less than or equal to the weak count, as each
   * memo reference implies a weak reference also.
   */
  std::atomic<unsigned> memoCount;

  /**
   * Freeze count. This is 0 if the object is not frozen, a thread id plus
   * on if the object is in the process of being frozen (with the id that of
   * the thread doing so), and the number of threads plus one if the process
   * of being frozen is complete.
   */
  std::atomic<unsigned> freezeCount;

  /**
   * Size of the object. This is set immediately after construction. A value
   * of zero is also indicative that the object is still being constructed.
   * Consequently, if the shared count reaches zero while the size is zero,
   * the object is not destroyed. This can happen when constructors create
   * shared pointers to `this`.
   */
  unsigned size;
};
}
