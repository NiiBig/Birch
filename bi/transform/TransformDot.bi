/*
 * Linear transformation of a multivariate random variate, as represented by
 * its associated distribution, which involves a dot product.
 *
 * - Value: Distribution type.
 */
class TransformDot<Value>(a:Expression<Real[_]>, x:Value,
    c:Expression<Real>) {
  /**
   * Scale.
   */
  a:Expression<Real[_]> <- a;
  
  /**
   * Distribution.
   */
  x:Value <- x;

  /**
   * Offset.
   */
  c:Expression<Real> <- c;
   
  function multiply(y:Expression<Real>) {
    a <- a*y;
    c <- c*y;
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
}

function TransformDot<Value>(a:Expression<Real[_]>, x:Value,
    c:Expression<Real>) -> TransformDot<Value> {
  m:TransformDot<Value>(a, x, c);
  return m;
}

function TransformDot<Value>(a:Expression<Real[_]>, x:Value) ->
    TransformDot<Value> {
  return TransformDot<Value>(a, x, Boxed(0.0));
}
