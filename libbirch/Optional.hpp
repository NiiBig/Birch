/**
 * @file
 */
#pragma once

namespace libbirch {
/**
 * Optional.
 *
 * @ingroup libbirch
 *
 * @tparam T Type.
 */
template<class T, class Enable = void>
class Optional {
  //
};
}

#include "libbirch/Nil.hpp"
#include "libbirch/Shared.hpp"
#include "libbirch/Weak.hpp"
#include "libbirch/Init.hpp"
#include "libbirch/type.hpp"

namespace libbirch {
/**
 * Optional for non-pointer types.
 *
 * @ingroup libbirch
 *
 * @tparam T Non-pointer type.
 */
template<class T>
class Optional<T,std::enable_if_t<!is_pointer<T>::value>> {
public:
  Optional(const Optional& o) = default;
  Optional(Optional&& o) = default;
  Optional& operator=(const Optional& o) = default;
  Optional& operator=(Optional&& o) = default;

  /**
   * Default constructor.
   */
  Optional() :
      value(),
      hasValue(false) {
    //
  }

  /**
   * Nil constructor.
   */
  Optional(const Nil&) :
      value(),
      hasValue(false) {
    //
  }

  /**
   * Generic value constructor.
   *
   * @tparam U Value type (convertible to @p T).
   */
  template<class U>
  Optional(const U& value) :
      value(value),
      hasValue(true) {
    //
  }

  /**
   * Generic copy constructor.
   *
   * @tparam U Value type (convertible to @p T).
   */
  template<class U>
  Optional(const Optional<U>& o) :
      hasValue(o.query()) {
    if (hasValue) {
      value = o.get();
    }
  }

  /**
   * Deep copy constructor.
   */
  Optional(Label* label, const Optional<T>& o) :
      value(label, o.value),
      hasValue(o.hasValue) {
    //
  }

  /**
   * Is there a value?
   */
  bool query() const {
    return hasValue;
  }

  /**
   * Get the value.
   */
  T& get() {
    libbirch_assert_msg_(hasValue, "optional has no value");
    return value;
  }

  /**
   * Get the value.
   */
  const T& get() const {
    libbirch_assert_msg_(hasValue, "optional has no value");
    return value;
  }

private:
  /**
   * The contained value, if any.
   */
  T value;

  /**
   * Is there a value?
   */
  bool hasValue;
};

/**
 * Optional for pointer types. Uses the pointer itself, set to `nullptr`, to
 * denote a missing value, rather than keeping a separate boolean flag.
 *
 * @ingroup libbirch
 *
 * @tparam P Pointer type.
 */
template<class P>
class Optional<P,std::enable_if_t<is_pointer<P>::value>> {
  template<class Q, class Enable> friend class Optional;
public:
  Optional() = default;
  Optional(const Optional& o) = default;
  Optional(Optional&& o) = default;
  Optional& operator=(const Optional& o) = default;
  Optional& operator=(Optional&& o) = default;

  /**
   * Generic conversion constructor.
   */
  template<class Q, typename = std::enable_if_t<is_pointer<Q>::value>>
  Optional(const Optional<Q>& o) : value(o.value) {
    //
  }

  /**
   * Nil constructor.
   */
  Optional(const Nil&) {
    //
  }

  /**
   * Deep copy constructor.
   */
  Optional(Label* label, const Optional<P>& o) :
      value(label, o.value) {
    //
  }

  /**
   * Generic value copy constructor.
   */
  template<class Q, typename = std::enable_if_t<is_pointer<Q>::value>>
  Optional(const Q& value) :
      value(value) {
    //
  }

  /**
   * Generic value move constructor.
   */
  template<class Q, typename = std::enable_if_t<is_pointer<Q>::value>>
  Optional(Q&& value) :
      value(std::move(value)) {
    //
  }

  /**
   * Nil assignment.
   */
  Optional<P>& operator=(const Nil& o) {
    value = nullptr;
    return *this;
  }

  /**
   * Generic value copy assignment.
   */
  template<class Q, typename = std::enable_if_t<is_pointer<Q>::value>>
  Optional<P>& operator=(const Q& value) {
    this->value = value;
    return *this;
  }

  /**
   * Generic value move assignment.
   */
  template<class Q, typename = std::enable_if_t<is_pointer<Q>::value>>
  Optional<P>& operator=(Q&& value) {
    this->value = std::move(value);
    return *this;
  }

  /**
   * Is there a value?
   */
  bool query() const {
    return value.query();
  }

  /**
   * Get the value.
   */
  P& get() {
    libbirch_assert_msg_(query(), "optional has no value");
    return value;
  }

  /**
   * Get the value.
   */
  const P& get() const {
    libbirch_assert_msg_(query(), "optional has no value");
    return value;
  }

private:
  /**
   * The value, `nullptr` value to denote no value.
   */
  P value;
};

template<class T>
struct is_value<Optional<T>> {
  static const bool value = is_value<T>::value;
};

template<class T>
void freeze(Optional<T>& o) {
  if (!is_value<T>::value && o.query()) {
    freeze(o.get());
  }
}

template<class T>
void thaw(Optional<T>& o, LazyLabel* label) {
  if (!is_value<T>::value && o.query()) {
    thaw(o.get(), label);
  }
}

template<class T>
void finish(Optional<T>& o) {
  if (!is_value<T>::value && o.query()) {
    finish(o.get());
  }
}

}
