/**
 * Delayed inverse-gamma random variate.
 */
class DelayInverseGamma(x:Random<Real>, α:Real, β:Real) < DelayValue<Real>(x) {
  /**
   * Shape.
   */
  α:Real <- α;

  /**
   * Scale.
   */
  β:Real <- β;

  function doSimulate() -> Real {
    return simulate_inverse_gamma(α, β);
  }
  
  function doObserve(x:Real) -> Real {
    return observe_inverse_gamma(x, α, β);
  }
}

function DelayInverseGamma(x:Random<Real>, α:Real, β:Real) ->
    DelayInverseGamma {
  m:DelayInverseGamma(x, α, β);
  return m;
}
