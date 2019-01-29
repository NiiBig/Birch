/**
 * @file
 */
#include "libbirch/Set.hpp"

static bi::Memo* const EMPTY = nullptr;

bi::Set::Set() :
    values(nullptr),
    nentries(0u),
    noccupied(0u) {
  //
}

bi::Set::~Set() {
  deallocate(values, nentries * sizeof(value_type));
}

bool bi::Set::empty() const {
  return nentries == 0u;
}

bool bi::Set::contains(const value_type value) {
  bool result = false;
  if (!empty()) {
    lock.share();
    unsigned i = hash(value);
    value_type v = values[i].load(std::memory_order_relaxed);
    while (v && v != value) {
      i = (i + 1u) & (nentries - 1u);
      v = values[i].load(std::memory_order_relaxed);
    }
    result = (v == value);
    lock.unshare();
  }
  return result;
}

void bi::Set::insert(const value_type value) {
  /* pre-condition */
  assert(value);

  reserve();
  lock.share();
  value_type expected = EMPTY;
  value_type desired = value;
  unsigned i = hash(value);
  while (!values[i].compare_exchange_strong(expected, desired)
      && expected != value) {
    i = (i + 1u) & (nentries - 1u);
    expected = EMPTY;
  }
  if (expected == value) {
    unreserve();  // value exists, cancel reservation for insert
  }
  lock.unshare();
}

unsigned bi::Set::hash(const value_type value) const {
  assert(nentries > 0u);
  return (reinterpret_cast<size_t>(value) >> 5ull) & (nentries - 1u);
}

unsigned bi::Set::crowd() const {
  /* the set is considered crowded if more than three-quarters of its
   * entries are occupied */
  return (nentries >> 1u) + (nentries >> 2u);
}

void bi::Set::reserve() {
  unsigned noccupied1 = noccupied.fetch_add(1u, std::memory_order_relaxed)
      + 1u;
  if (noccupied1 > crowd()) {
    /* obtain resize lock */
    lock.keep();

    /* check that no other thread has resized in the meantime */
    if (noccupied1 > crowd()) {
      /* save previous contents */
      unsigned nentries1 = nentries;
      value_type* values1 = (value_type*)values;

      /* initialize new contents */
      unsigned nentries2 = std::max(2u * nentries1, INITIAL_SET_SIZE);
      value_type* values2 = (value_type*)allocate(
          nentries2 * sizeof(value_type));
      std::memset(values2, 0, nentries2 * sizeof(value_type));

      /* copy contents from previous set */
      nentries = nentries2;  // set this here as needed by hash()
      for (unsigned i = 0u; i < nentries1; ++i) {
        value_type value = values1[i];
        if (value != EMPTY) {
          /* rehash and insert */
          unsigned j = hash(value);
          while (values2[j]) {
            j = (j + 1u) & (nentries2 - 1u);
          }
          values2[j] = value;
        }
      }

      /* update object */
      values = (std::atomic<value_type>*)values2;

      /* deallocate previous table */
      deallocate(values1, nentries1 * sizeof(value_type));
    }

    /* release resize lock */
    lock.unkeep();
  }
}

void bi::Set::unreserve() {
  noccupied.fetch_sub(1u, std::memory_order_relaxed);
}
