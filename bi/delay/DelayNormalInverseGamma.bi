/**
 * Normal-inverse-gamma random variable with delayed sampling.
 */
class DelayNormalInverseGamma(x:Random<Real>, μ:Real, a2:Real,
    σ2:InverseGamma) < DelayValue<Real>(x) {
  /**
   * Mean.
   */
  μ:Real <- μ;

  /**
   * Variance.
   */
  a2:Real <- a2;

  /**
   * Scale.
   */
  σ2:DelayInverseGamma <- σ2;

  function doSimulate() -> Real {
    return simulate_normal_inverse_gamma(μ, a2, σ2.α, σ2.β);
  }
  
  function doObserve(x:Real) -> Real {
    return observe_normal_inverse_gamma(x, μ, a2, σ2.α, σ2.β);
  }
}
