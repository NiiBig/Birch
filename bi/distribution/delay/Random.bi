/**
 * Random variate.
 *
 * - Value: Value type.
 */
class Random<Value> < Delay {
  /**
   * Value.
   */
  x:Value?;

  /**
   * Value conversion.
   */
  operator -> Value {
    return value();
  }

  /**
   * Value assignment.
   */
  operator <- x:Value {
    assert isUninitialized();
    set(x);
    realize();
  }

  /**
   * Optional value assignment.
   */
  operator <- x:Value? {
    assert isUninitialized();
    if (x?) {
      set(x!);
      realize();
    }
  }
  
  /**
   * Get the value of the random variable, forcing its instantiation if
   * it has not already been instantiated.
   */
  function value() -> Value {
    if (isMissing()) {
      realize();
    }
    assert x?;
    return x!;
  }

  /**
   * Is the value of the random variable missing?
   */
  function isMissing() -> Boolean {
    return !(x?);
  }

  function set(x:Value) {
    this.x <- x;
  }
  
  function setWeight(w:Real) {
    this.w <- w;
  }
  
  /**
   * Simulate the random variable.
   */
  function simulate() -> Value {
    realize();
    return value();
  }
  
  /**
   * Observe the random variable.
   *
   * - x: The observed value.
   *
   * Returns: the log likelihood.
   */
  function observe(x:Value) -> Real {
    set(x);
    realize();
    return w;
  }
}
