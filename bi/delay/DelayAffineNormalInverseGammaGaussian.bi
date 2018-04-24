/**
 * Affine-normal-inverse-gamma-Gaussian random variable with delayed
 * sampling.
 */
class DelayAffineNormalInverseGammaGaussian(x:Random<Real>, a:Real,
    μ:DelayNormalInverseGamma, c:Real) < DelayValue<Real>(x) {
  /**
   * Scale.
   */
  a:Real;
    
  /**
   * Mean.
   */
  μ:DelayNormalInverseGamma <- μ;

  /**
   * Offset.
   */
  c:Real <- c;

  function doSimulate() -> Real {
    return simulate_affine_normal_inverse_gamma_gaussian(a, μ.μ, c, μ.a2, μ.σ2.α, μ.σ2.β);
  }
  
  function doObserve(x:Real) -> Real {
    return observe_normal_inverse_gamma_gaussian(x, a, μ.μ, c, μ.a2, μ.σ2.α,
        μ.σ2.β);
  }

  function doCondition(x:Real) -> Real {
    (μ.μ, μ.a2, μ.σ2.α, μ.σ2.β) <- observe_normal_inverse_gamma_gaussian(x,
        a, μ.μ, c, μ.a2, μ.σ2.α, μ.σ2.β);
  }
}
