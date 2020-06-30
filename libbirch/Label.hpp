/**
 * @file
 */
#pragma once

#include "libbirch/Any.hpp"
#include "libbirch/Memo.hpp"

namespace libbirch {
/**
 * Label for bookkeeping lazy deep clones.
 *
 * @ingroup libbirch
 */
class Label : public Any {
public:
  /**
   * Constructor.
   */
  Label();

  /**
   * Copy constructor.
   */
  Label(const Label& o);

  /**
   * Update a smart pointer for writing.
   *
   * @param o Smart pointer (Shared, Weak or Init).
   */
  template<class P>
  auto get(P& o)  {
    auto ptr = o.get();
    if (ptr && ptr->isFrozen()) {  // isFrozen a useful guard for performance
      lock.setWrite();
      ptr = o.get();  // reload now that within critical region
      auto old = ptr;
      ptr = static_cast<typename P::value_type*>(mapGet(old));
      if (ptr != old) {
        o.replace(ptr);
      }
      lock.unsetWrite();
    }
    return ptr;
  }

  /**
   * Update a smart pointer for reading.
   *
   * @param o Smart pointer (Shared, Weak or Init).
   */
  template<class P>
  auto pull(P& o) {
    auto ptr = o.get();
    if (ptr && ptr->isFrozen()) {  // isFrozen a useful guard for performance
      lock.setRead();
      ptr = o.get();  // reload now that within critical region
      auto old = ptr;
      ptr = static_cast<typename P::value_type*>(mapPull(old));
      if (ptr != old) {
        o.replace(ptr);
      }
      lock.unsetRead();
    }
    return ptr;
  }

  /**
   * Map a raw pointer for writing.
   *
   * @param ptr Raw pointer.
   */
  template<class T>
  auto get(T* ptr)  {
    if (ptr && ptr->isFrozen()) {  // isFrozen a useful guard for performance
      lock.setWrite();
      ptr = static_cast<T*>(mapGet(ptr));
      lock.unsetWrite();
    }
    return ptr;
  }

  /**
   * Map a raw pointer for reading.
   *
   * @param ptr Raw pointer.
   */
  template<class T>
  auto pull(T* ptr) {
    if (ptr && ptr->isFrozen()) {  // isFrozen a useful guard for performance
      lock.setRead();
      ptr = static_cast<T*>(mapPull(ptr));
      lock.unsetRead();
    }
    return ptr;
  }

protected:
  virtual uint16_t size_() const override {
    return (uint16_t)sizeof(*this);
  }

  virtual void finish_(libbirch::Label* label) override {
    lock.setRead();
    memo.finish(label);
    lock.unsetRead();
  }

  virtual void freeze_() override {
    lock.setRead();
    memo.freeze();
    lock.unsetRead();
  }

  virtual Label* copy_(libbirch::Label* label) const override {
    return new Label(*this);
  }

  virtual Label* recycle_(libbirch::Label* label) override {
    return this;
  }

  virtual void mark_() override {
    lock.setRead();
    memo.mark();
    lock.unsetRead();
  }

  virtual void scan_(const bool reachable) override {
    lock.setRead();
    memo.scan(reachable);
    lock.unsetRead();
  }

  virtual void collect_() override {
    lock.setRead();
    memo.collect();
    lock.unsetRead();
  }

  virtual bi::type::String getClassName() const {
    return "Label";
  }

private:
  /**
   * Map an object that may not yet have been cloned, cloning it if
   * necessary.
   */
  Any* mapGet(Any* o);

  /**
   * Map an object that may not yet have been cloned, without cloning it.
   * This is used as an optimization for read-only access.
   */
  Any* mapPull(Any* o);

  /**
   * Memo that maps source objects to clones.
   */
  Memo memo;

  /**
   * Lock.
   */
  ReadersWriterLock lock;
};
}
