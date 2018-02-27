/**
 * Default floating point.
 */
type Real = Real64;

/**
 * Convert to Real.
 */
function Real(x:Real64) -> Real {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:Real32) -> Real {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:Integer64) -> Real {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:Integer32) -> Real {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:String) -> Real {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:Real64?) -> Real? {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:Real32?) -> Real? {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:Integer64?) -> Real? {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:Integer32?) -> Real? {
  return Real64(x);
}

/**
 * Convert to Real.
 */
function Real(x:String?) -> Real? {
  return Real64(x);
}
