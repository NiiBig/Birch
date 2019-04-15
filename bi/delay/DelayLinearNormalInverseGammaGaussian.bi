/*
 * Delayed linear-normal-inverse-gamma-Gaussian random variate.
 */
final class DelayLinearNormalInverseGammaGaussian(x:Random<Real>&, a:Real,
    μ:DelayNormalInverseGamma, c:Real) < DelayValue<Real>(x) {
  /**
   * Scale.
   */
  a:Real <- a;
    
  /**
   * Mean.
   */
  μ:DelayNormalInverseGamma& <- μ;

  /**
   * Offset.
   */
  c:Real <- c;

  function simulate() -> Real {
    return simulate_linear_normal_inverse_gamma_gaussian(a, μ!.μ, c,
        1.0/μ!.λ, μ!.σ2!.α, μ!.σ2!.β);
  }
  
  function observe(x:Real) -> Real {
    return observe_linear_normal_inverse_gamma_gaussian(x, a, μ!.μ, c,
        1.0/μ!.λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function update(x:Real) {
    (μ!.μ, μ!.λ, μ!.σ2!.α, μ!.σ2!.β) <- update_linear_normal_inverse_gamma_gaussian(
        x, a, μ!.μ, c, μ!.λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function downdate(x:Real) {
    (μ!.μ, μ!.λ, μ!.σ2!.α, μ!.σ2!.β) <- downdate_linear_normal_inverse_gamma_gaussian(
        x, a, μ!.μ, c, μ!.λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function pdf(x:Real) -> Real {
    return pdf_linear_normal_inverse_gamma_gaussian(x, a, μ!.μ, c,
        1.0/μ!.λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function cdf(x:Real) -> Real {
    return cdf_linear_normal_inverse_gamma_gaussian(x, a, μ!.μ, c,
        1.0/μ!.λ, μ!.σ2!.α, μ!.σ2!.β);
  }
}

function DelayLinearNormalInverseGammaGaussian(x:Random<Real>&, a:Real,
    μ:DelayNormalInverseGamma, c:Real) ->
    DelayLinearNormalInverseGammaGaussian {
  m:DelayLinearNormalInverseGammaGaussian(x, a, μ, c);
  μ.setChild(m);
  return m;
}
