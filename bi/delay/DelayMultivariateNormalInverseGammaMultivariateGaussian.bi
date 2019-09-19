/*
 * Delayed multivariate normal-inverse-gamma-Gaussian random variate.
 */
final class DelayMultivariateNormalInverseGammaGaussian(future:Real[_]?,
    futureUpdate:Boolean, μ:DelayMultivariateNormalInverseGamma) <
    DelayValue<Real[_]>(future, futureUpdate) {
  /**
   * Mean.
   */
  μ:DelayMultivariateNormalInverseGamma& <- μ;

  function simulate() -> Real[_] {
    return simulate_identical_normal_inverse_gamma_gaussian(
        solve(μ!.Λ, μ!.ν), μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }
  
  function logpdf(x:Real[_]) -> Real {
    return logpdf_identical_normal_inverse_gamma_gaussian(x,
        solve(μ!.Λ, μ!.ν), μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function update(x:Real[_]) {
    (μ!.ν, μ!.Λ, μ!.γ, μ!.σ2!.α, μ!.σ2!.β) <- update_identical_normal_inverse_gamma_gaussian(
        x, μ!.ν, μ!.Λ, μ!.γ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function downdate(x:Real[_]) {
    (μ!.ν, μ!.Λ, μ!.γ, μ!.σ2!.α, μ!.σ2!.β) <- downdate_identical_normal_inverse_gamma_gaussian(
        x, μ!.ν, μ!.Λ, μ!.γ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function pdf(x:Real[_]) -> Real {
    return pdf_identical_normal_inverse_gamma_gaussian(x,
        solve(μ!.Λ, μ!.ν), μ!.Λ, μ!.σ2!.α, μ!.σ2!.β);
  }

  function write(buffer:Buffer) {
    buffer.set(value());
  }
}

function DelayMultivariateNormalInverseGammaGaussian(future:Real[_]?,
    futureUpdate:Boolean, μ:DelayMultivariateNormalInverseGamma) ->
    DelayMultivariateNormalInverseGammaGaussian {
  m:DelayMultivariateNormalInverseGammaGaussian(future, futureUpdate, μ);
  μ.setChild(m);
  return m;
}
