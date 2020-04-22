/**
 * Lazy `exp`.
 */
final class Exp<Argument,Value>(x:Expression<Argument>) <
    UnaryExpression<Argument,Value>(x) {
  override function computeValue(x:Argument) -> Value {
    return exp(x);
  }

  override function computeGrad(d:Value, x:Argument) -> Argument {
    return d*exp(x);
  }
}

/**
 * Lazy `exp`.
 */
function exp(x:Expression<Real>) -> Exp<Real,Real> {
  m:Exp<Real,Real>(x);
  return m;
}
