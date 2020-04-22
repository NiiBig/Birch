/**
 * Lazy `cosh`.
 */
final class Cosh<Argument,Value>(x:Expression<Argument>) <
    UnaryExpression<Argument,Value>(x) {
  override function computeValue(x:Argument) -> Value {
    return cosh(x);
  }

  override function computeGrad(d:Value, x:Argument) -> Argument {
    return -d*sinh(x);
  }
}

/**
 * Lazy `cosh`.
 */
function cosh(x:Expression<Real>) -> Cosh<Real,Real> {
  m:Cosh<Real,Real>(x);
  return m;
}
