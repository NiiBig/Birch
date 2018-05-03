/*
 * Linear transformation of a normal-inverse-gamma variate.
 */
class TransformLinearNormalInverseGamma(a:Real, x:DelayNormalInverseGamma,
    c:Real) < TransformLinear(a, c) {
  /**
   * Random variate.
   */
  x:DelayNormalInverseGamma <- x;
}

function TransformLinearNormalInverseGamma(a:Real, x:DelayNormalInverseGamma,
    c:Real) -> TransformLinearNormalInverseGamma {
  m:TransformLinearNormalInverseGamma(a, x, c);
  return m;
}
