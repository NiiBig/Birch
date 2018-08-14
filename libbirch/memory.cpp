/**
 * @file
 */
#include "libbirch/memory.hpp"

#include <unistd.h>

#if !DISABLE_POOL
char* bi::buffer = heap();
bi::Pool bi::pool[64];
#endif

char* bi::heap() {
  /* determine a preferred size of the heap based on total physical memory */
  size_t size = sysconf(_SC_PAGE_SIZE);
  size_t npages = sysconf(_SC_PHYS_PAGES);
  size_t n = 8u*npages*size;

  /* attemp tto allocate this amount, successively halving until
   * successful */
  void* ptr = nullptr;
  do {
    posix_memalign(&ptr, 64ull, n);
    n >>= 1;
  } while (!ptr && n > 0u);
  assert(ptr);

  return (char*)ptr;
}

void* bi::allocate(const size_t n) {
#if DISABLE_POOL
  return std::malloc(n);
#else
  void* ptr = nullptr;
  if (n > 0ull) {
    int i = bin(n);       // determine which pool
    ptr = pool[i].pop();  // attempt to reuse from this pool
    if (!ptr) {           // otherwise allocate new
      size_t m = unbin(i);
      #pragma omp atomic capture
      {
        ptr = buffer;
        buffer += m;
      }
    }
    assert(ptr);
  }
  return ptr;
#endif
}

void bi::deallocate(void* ptr, const size_t n) {
#if DISABLE_POOL
  std::free(ptr);
#else
  if (n > 0ull) {
    assert(ptr);
    int i = bin(n);
    pool[i].push(ptr);
  }
#endif
}

void bi::deallocate(void* ptr, const unsigned n) {
#if DISABLE_POOL
  std::free(ptr);
#else
  if (n > 0u) {
    assert(ptr);
    int i = bin(n);
    pool[i].push(ptr);
  }
#endif
}

void* bi::reallocate(void* ptr1, const size_t n1, const size_t n2) {
#if DISABLE_POOL
  return std::realloc(ptr1, n2);
#else
  int i1 = bin(n1);
  int i2 = bin(n2);
  void* ptr2 = ptr1;
  if (i1 != i2) {
    /* can't continue using current allocation */
    ptr2 = allocate(n2);
    std::memcpy(ptr2, ptr1, n1);
    deallocate(ptr1, n1);
  }
  return ptr2;
#endif
}
