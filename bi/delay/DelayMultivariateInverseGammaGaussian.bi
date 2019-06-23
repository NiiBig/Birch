/*
 * Delayed multivariate normal-inverse-gamma-Gaussian random variate.
 */
final class DelayMultivariateInverseGammaGaussian(future:Real[_]?,
    futureUpdate:Boolean, μ:Real[_], σ2:DelayInverseGamma) <
    DelayValue<Real[_]>(future, futureUpdate) {
  /**
   * Mean.
   */
  μ:Real[_] <- μ;

  /**
   * Variance.
   */
  σ2:DelayInverseGamma& <- σ2;

  function simulate() -> Real[_] {
    return simulate_multivariate_inverse_gamma_gaussian(μ, σ2!.α, σ2!.β);
  }
  
  function logpdf(x:Real[_]) -> Real {
    return logpdf_multivariate_inverse_gamma_gaussian(x, μ, σ2!.α, σ2!.β);
  }

  function update(x:Real[_]) {
    (σ2!.α, σ2!.β) <- update_multivariate_inverse_gamma_gaussian(x, μ,
        σ2!.α, σ2!.β);
  }

  function downdate(x:Real[_]) {
    (σ2!.α, σ2!.β) <- downdate_multivariate_inverse_gamma_gaussian(x, μ,
        σ2!.α, σ2!.β);
  }

  function pdf(x:Real[_]) -> Real {
    return pdf_multivariate_inverse_gamma_gaussian(x, μ, σ2!.α, σ2!.β);
  }

  function write(buffer:Buffer) {
    buffer.set(value());
  }
}

function DelayMultivariateInverseGammaGaussian(future:Real[_]?,
    futureUpdate:Boolean, μ:Real[_], σ2:DelayInverseGamma) ->
    DelayMultivariateInverseGammaGaussian {
  m:DelayMultivariateInverseGammaGaussian(future, futureUpdate, μ, σ2);
  σ2.setChild(m);
  return m;
}
