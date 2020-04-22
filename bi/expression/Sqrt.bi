/**
 * Lazy `sqrt`.
 */
final class Sqrt<Argument,Value>(x:Expression<Argument>) <
    UnaryExpression<Argument,Value>(x) {
  function doValue(x:Argument) -> Value {
    return sqrt(x);
  }

  function doGrad(d:Value, x:Argument) -> Argument {
    return d*0.5/sqrt(x);
  }
}

/**
 * Lazy `sqrt`.
 */
function sqrt(x:Expression<Real>) -> Sqrt<Real,Real> {
  m:Sqrt<Real,Real>(x);
  return m;
}
