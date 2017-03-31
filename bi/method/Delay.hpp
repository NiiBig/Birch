/**
 * @file
 */
#pragma once

#include "bi/bi.hpp"
#include "bi/method/DelayInterface.hpp"
#include "bi/method/DelayState.hpp"

namespace bi {
/**
 * Delay variate.
 *
 * @tparam Value Value type.
 * @tparam Distribution Distribution type.
 */
template<class Value, class Distribution>
class Delay: public virtual DelayInterface {
public:
  /**
   * Value type.
   */
  typedef typename value_type<Value>::type value_type;

  /**
   * Distribution type.
   */
  typedef Distribution distribution_type;

  /**
   * Constructor.
   */
  Delay();

  /**
   * Value assigment.
   */
  Delay<Value,Distribution>& operator=(const value_type& o);

  /**
   * Distribution assigment.
   */
  Delay<Value,Distribution>& operator=(const distribution_type& o);

  /**
   * Cast to value type.
   */
  operator value_type&();

  /**
   * Cast to value type.
   */
  operator const value_type&() const;

  /**
   * Cast to distribution type.
   */
  explicit operator Distribution&();

  /**
   * Cast to distribution type.
   */
  explicit operator const Distribution&() const;

  /*
   * DelayInterface requirements.
   */
  virtual void simulate();
  virtual double observe();
  virtual int getId() const;
  virtual void setId(const int id);
  virtual DelayState getState() const;
  virtual void setState(const DelayState state);

  /**
   * Value.
   */
  Value x;

  /**
   * Distribution.
   */
  Distribution m;

  /**
   * Delay variate id, or -1 if this has not been assigned.
   */
  int id;

  /**
   * Delay variate state, taking one of the values of the enum State.
   */
  DelayState state;
};
}

#include "bi/method/Method.hpp"

template<class Value, class Distribution>
bi::Delay<Value,Distribution>::Delay() :
    id(-1),
    state(MISSING) {
  //
}

template<class Value, class Distribution>
bi::Delay<Value,Distribution>& bi::Delay<Value,Distribution>::operator=(
    const value_type& o) {
  x = o;
  state = ASSIGNED;

  return *this;
}

template<class Value, class Distribution>
bi::Delay<Value,Distribution>& bi::Delay<Value,Distribution>::operator=(
    const distribution_type& o) {
  m = o;
  state = MISSING;
  id = method->add(this);

  return *this;
}

template<class Value, class Distribution>
bi::Delay<Value,Distribution>::operator value_type&() {
  if (id >= 0 && state == MISSING) {
    method->simulate(id);
  }
  return x;
}

template<class Value, class Distribution>
bi::Delay<Value,Distribution>::operator const value_type&() const {
  if (id >= 0 && state == MISSING) {
    method->simulate(id);
  }
  return x;
}

template<class Value, class Distribution>
bi::Delay<Value,Distribution>::operator Distribution&() {
  if (id >= 0 && state == MISSING) {
    return m;
  } else {
    throw std::bad_cast();
  }
}

template<class Value, class Distribution>
bi::Delay<Value,Distribution>::operator const Distribution&() const {
  if (id >= 0 && state == MISSING) {
    return m;
  } else {
    throw std::bad_cast();
  }
}

template<class Value, class Distribution>
void bi::Delay<Value,Distribution>::simulate() {
  left_tilde_(x, m);
}

template<class Value, class Distribution>
double bi::Delay<Value,Distribution>::observe() {
  return right_tilde_(x, m);
}

template<class Value, class Distribution>
int bi::Delay<Value,Distribution>::getId() const {
  return this->id;
}

template<class Value, class Distribution>
void bi::Delay<Value,Distribution>::setId(const int id) {
  this->id = id;
}

template<class Value, class Distribution>
bi::DelayState bi::Delay<Value,Distribution>::getState() const {
  return this->state;
}

template<class Value, class Distribution>
void bi::Delay<Value,Distribution>::setState(const DelayState state) {
  this->state = state;
}
