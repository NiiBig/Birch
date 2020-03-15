/*
 * Linear transformation of a random variate, as represented by its
 * associated distribution.
 *
 * - Value: Distribution type.
 */
class TransformLinear<Value>(a:Expression<Real>, x:Value,
    c:Expression<Real>) {
  /**
   * Scale.
   */
  a:Expression<Real> <- a;

  /**
   * Distribution.
   */
  x:Value <- x;

  /**
   * Offset.
   */
  c:Expression<Real> <- c;
  
  function multiply(y:Expression<Real>) {
    a <- y*a;
    c <- y*c;
  }

  function divide(y:Expression<Real>) {
    a <- a/y;
    c <- c/y;
  }

  function add(y:Expression<Real>) {
    c <- c + y;
  }

  function subtract(y:Expression<Real>) {
    c <- c - y;
  }
  
  function negateAndAdd(y:Expression<Real>) {
    a <- -a;
    c <- y - c;
  }

  /**
   * Is the transformation valid? This evaluates the scale and offset. It 
   * then returns true if the Distribution object remains uninstantiated, and
   * false otherwise (which would mean that either or both of the scale and
   * offset depend it).
   */
  function isValid() -> Boolean {
    a.value();
    c.value();
    return !x.hasValue();
  }
}

function TransformLinear<Value>(a:Expression<Real>, x:Value,
    c:Expression<Real>) -> TransformLinear<Value> {
  m:TransformLinear<Value>(a, x, c);
  return m;
}

function TransformLinear<Value>(a:Expression<Real>, x:Value) ->
    TransformLinear<Value> {
  return TransformLinear<Value>(a, x, Boxed(0.0));
}
