/**
 * 32-bit signed integer.
 */
type Integer32 < Integer64;

/**
 * Convert to Integer32.
 */
function Integer32(x:Real64) -> Integer32 {
  cpp{{
  return static_cast<bi::type::Integer32_>(x_);
  }}
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Real32) -> Integer32 {
  cpp{{
  return static_cast<bi::type::Integer32_>(x_);
  }}
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Integer64) -> Integer32 {
  cpp{{
  return static_cast<bi::type::Integer32_>(x_);
  }}
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Integer32) -> Integer32 {
  return x;
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Integer16) -> Integer32 {
  cpp{{
  return static_cast<bi::type::Integer32_>(x_);
  }}
}

/**
 * Convert to Integer32.
 */
function Integer32(x:String) -> Integer32 {
  cpp{{
  return ::atoi(x_.c_str());
  }}
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Real64?) -> Integer32? {
  if (x?) {
    return Integer32(x!);
  } else {
    return nil;
  }
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Real32?) -> Integer32? {
  if (x?) {
    return Integer32(x!);
  } else {
    return nil;
  }
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Integer64?) -> Integer32? {
  if (x?) {
    return Integer32(x!);
  } else {
    return nil;
  }
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Integer32?) -> Integer32? {
  return x;
}

/**
 * Convert to Integer32.
 */
function Integer32(x:Integer16?) -> Integer32? {
  if (x?) {
    return Integer32(x!);
  } else {
    return nil;
  }
}

/**
 * Convert to Integer32.
 */
function Integer32(x:String?) -> Integer32? {
  if (x?) {
    return Integer32(x!);
  } else {
    return nil;
  }
}

/*
 * Operators
 */
operator (x:Integer32 + y:Integer32) -> Integer32;
operator (x:Integer32 - y:Integer32) -> Integer32;
operator (x:Integer32 * y:Integer32) -> Integer32;
operator (x:Integer32 / y:Integer32) -> Integer32;
operator (+x:Integer32) -> Integer32;
operator (-x:Integer32) -> Integer32;
operator (x:Integer32 > y:Integer32) -> Boolean;
operator (x:Integer32 < y:Integer32) -> Boolean;
operator (x:Integer32 >= y:Integer32) -> Boolean;
operator (x:Integer32 <= y:Integer32) -> Boolean;
operator (x:Integer32 == y:Integer32) -> Boolean;
operator (x:Integer32 != y:Integer32) -> Boolean;

/**
 * Absolute value.
 */
function abs(x:Integer32) -> Integer32 {
  cpp {{
  return std::abs(x_);
  }}
}

/**
 * Modulus.
 */
function mod(x:Integer32, y:Integer32) -> Integer32 {
  cpp {{
  return x_ % y_;
  }}
}

/**
 * Maximum of two values.
 */
function max(x:Integer32, y:Integer32) -> Integer32 {
  cpp {{
  return std::max(x_, y_);
  }}
}

/**
 * Minimum of two values.
 */
function min(x:Integer32, y:Integer32) -> Integer32 {
  cpp {{
  return std::min(x_, y_);
  }}
}

/**
 * Round an integer up to the next power of two. Zero and negative integers
 * return zero.
 */
function pow2(x:Integer32) -> Integer32 {
  if (x < 0) {
    return Integer32(0);
  } else {
    y:Integer32 <- x/2;
    z:Integer32 <- 1;
    while (y > 0) {
      y <- y/Integer32(2);
      z <- z*Integer32(2);
    }
    return z;
  }
}
