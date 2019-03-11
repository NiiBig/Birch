/**
 * @file
 */
#include "libbirch/ExclusiveLock.hpp"

void libbirch::ExclusiveLock::keep() {
  /* spin until exclusive lock obtained */
  bool expected;
  do {
    expected = false;
  } while (!lock.compare_exchange_weak(expected, true, std::memory_order_seq_cst));
}
