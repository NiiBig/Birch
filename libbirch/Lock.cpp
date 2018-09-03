/**
 * @file
 */
#include "libbirch/Lock.hpp"

void bi::Lock::share() {
  /* spin until exclusive lock is released and shared count updated */
  joint_lock_type expected = lock.joint.load(), desired;
  do {
    expected.keepCount = 0;
    desired = {expected.shareCount + 1, 0};
  } while (!lock.joint.compare_exchange_weak(expected, desired));
}

void bi::Lock::unshare() {
  lock.split.shareCount.fetch_sub(1u);
}

void bi::Lock::keep() {
  /* spin until exclusive lock obtained */
  unsigned expected;
  do {
    expected = 0u;
  } while (!lock.split.keepCount.compare_exchange_weak(expected, 1u));

  /* spin until all threads with shared locks release */
  while (lock.split.shareCount.load() > 0u);
}

void bi::Lock::unkeep() {
  lock.split.keepCount.store(0u);
}
