/**
 * Lazy `pow`.
 */
final class Pow<Left,Right,Value>(left:Expression<Left>,
    right:Expression<Right>) < BinaryExpression<Left,Right,Value>(left, right) {  
  function doValue(l:Left, r:Right) -> Value {
    return pow(l, r);
  }

  function doGradient(d:Value, l:Left, r:Right) -> (Left, Right) {
    assert l > 0;
    return (d*r*pow(l, r - 1), d*pow(l, r)*log(l));
  }
}

/**
 * Lazy `pow`.
 */
function pow(x:Expression<Real>, y:Expression<Real>) -> Pow<Real,Real,Real> {
  m:Pow<Real,Real,Real>(x, y);
  return m;
}

/**
 * Lazy `pow`.
 */
function pow(x:Real, y:Expression<Real>) -> Pow<Real,Real,Real> {
  return pow(Boxed(x), y);
}

/**
 * Lazy `pow`.
 */
function pow(x:Expression<Real>, y:Real) -> Pow<Real,Real,Real> {
  return pow(x, Boxed(y));
}
