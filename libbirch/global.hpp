/**
 * @file
 */
#pragma once

#include "libbirch/external.hpp"

namespace libbirch {
class Label;

inline int get_max_threads() {
#ifdef _OPENMP
  return omp_get_max_threads();
#else
  return 1;
#endif
}

inline int get_thread_num() {
#ifdef _OPENMP
  return omp_get_thread_num();
#else
  return 0;
#endif
}

extern Label* rootLabel;
}
