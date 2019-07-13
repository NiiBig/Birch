/**
 * @file
 */
#pragma once
#if !ENABLE_LAZY_DEEP_CLONE

#include "libbirch/Any.hpp"

namespace libbirch {
/**
 * Hash table of object mappings.
 *
 * @ingroup libbirch
 */
class EagerMemo {
public:
  /**
   * Key type.
   */
  using key_type = Any*;

  /**
   * Value type.
   */
  using value_type = Any*;

  /**
   * Constructor.
   */
  EagerMemo();

  /**
   * Destructor.
   */
  ~EagerMemo();

  /**
   * Is this empty?
   */
  bool empty() const;

  /**
   * Get a value.
   *
   * @param key Key.
   *
   * @return If @p key exists, then its associated value, otherwise
   * @p failed.
   */
  value_type get(const key_type key, const value_type failed = nullptr);

  /**
   * Put an entry.
   *
   * @param key Key.
   * @param value Value.
   */
  void put(const key_type key, const value_type value);

private:
  /**
   * Compute the hash code for a given key for a table with the given number
   * of entries.
   */
  static unsigned hash(const key_type key, const unsigned nentries);

  /**
   * Compute the lower bound on reserved entries to be considered crowded.
   */
  unsigned crowd() const;

  /**
   * Reserve space for a (possible) new entry, resizing if necessary.
   */
  void reserve();
  
  /**
   * Rehash the table.
   */
  void rehash();

  /**
   * The keys.
   */
  key_type* keys;

  /**
   * The values.
   */
  value_type* values;

  /**
   * Number of entries in the table.
   */
  unsigned nentries;

  /**
   * Id of the thread that allocated keys and values.
   */
  unsigned tentries;

  /**
   * Number of occupied entries in the table.
   */
  unsigned noccupied;
};
}

inline bool libbirch::EagerMemo::empty() const {
  return nentries == 0u;
}

inline unsigned libbirch::EagerMemo::hash(const key_type key, const unsigned nentries) {
  assert(nentries > 0u);
  return static_cast<unsigned>(reinterpret_cast<size_t>(key) >> 6ull)
      & (nentries - 1u);
}

inline unsigned libbirch::EagerMemo::crowd() const {
  /* the table is considered crowded if more than three-quarters of its
   * entries are occupied */
  return (nentries >> 1u) + (nentries >> 2u);
}

#endif
