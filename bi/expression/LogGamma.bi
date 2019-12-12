/**
 * Lazy `lgamma`.
 */
final class LogGamma<Argument,Value>(x:Expression<Argument>) <
    UnaryExpression<Argument,Value>(x) {
  function graft() -> Expression<Value> {
    return lgamma(single.graft());
  }

  function doValue(x:Argument) -> Value {
    return lgamma(x);
  }

  function doGradient(d:Value, x:Argument) -> Argument {
    return d*digamma(x);
  }
}

/**
 * Lazy `lgamma`.
 */
function lgamma(x:Expression<Real>) -> LogGamma<Real,Real> {
  m:LogGamma<Real,Real>(x);
  return m;
}
