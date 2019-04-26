/*
 * Delayed dot-normal-inverse-gamma-Gaussian random variate. This is
 * univariate, where the prior over the mean is given by a dot product with a
 * multivariate normal inverse gamma random variable, plus scalar.
 */
final class DelayMultivariateDotNormalInverseGammaGaussian(future:Real?,
    futureUpdate:Boolean, a:Real[_], μ:DelayMultivariateNormalInverseGamma,
    c:Real) < DelayValue<Real>(future, futureUpdate) {
  /**
   * Scale.
   */
  a:Real[_] <- a;

  /**
   * Mean.
   */
  μ:DelayMultivariateNormalInverseGamma& <- μ;

  /**
   * Offset.
   */
  c:Real <- c;

  function simulate() -> Real {
    return simulate_multivariate_dot_normal_inverse_gamma_gaussian(a, μ!.μ,
        c, μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }
  
  function observe(x:Real) -> Real {
    return observe_multivariate_dot_normal_inverse_gamma_gaussian(x, a, μ!.μ,
        c, μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function update(x:Real) {
    (μ!.μ, μ!.Λ, μ!.σ2!.α, μ!.σ2!.β) <- update_multivariate_dot_normal_inverse_gamma_gaussian(
        x, a, μ!.μ, c, μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function downdate(x:Real) {
    (μ!.μ, μ!.Λ, μ!.σ2!.α, μ!.σ2!.β) <- downdate_multivariate_dot_normal_inverse_gamma_gaussian(
        x, a, μ!.μ, c, μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function pdf(x:Real) -> Real {
    return pdf_multivariate_dot_normal_inverse_gamma_gaussian(x, a, μ!.μ, c,
        μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function cdf(x:Real) -> Real {
    return cdf_multivariate_dot_normal_inverse_gamma_gaussian(x, a, μ!.μ, c,
        μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }
}

function DelayMultivariateDotNormalInverseGammaGaussian(future:Real?,
    futureUpdate:Boolean, a:Real[_], μ:DelayMultivariateNormalInverseGamma,
    c:Real) -> DelayMultivariateDotNormalInverseGammaGaussian {
  m:DelayMultivariateDotNormalInverseGammaGaussian(future, futureUpdate, a, μ, c);
  μ.setChild(m);
  return m;
}
