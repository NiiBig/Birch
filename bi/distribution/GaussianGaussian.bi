/*
 * Grafted Gaussian-Gaussian distribution.
 */
final class GaussianGaussian(m:Gaussian, s2:Expression<Real>) <
    GraftedGaussian(m.μ, m.σ2 + s2) {
  /**
   * Mean.
   */
  m:Gaussian& <- m;

  /**
   * Variance.
   */
  s2:Expression<Real> <- s2;

  function update(x:Real) {
    (m.μ, m.σ2) <- update_gaussian_gaussian(x, m.μ.value(), m.σ2.value(), s2.value());
  }

  function downdate(x:Real) {
    (m.μ, m.σ2) <- downdate_gaussian_gaussian(x, m.μ.value(), m.σ2.value(), s2.value());
  }

  function updateLazy(x:Expression<Real>) {
    (m.μ, m.σ2) <- update_lazy_gaussian_gaussian(x, m.μ, m.σ2, s2);
  }
}

function GaussianGaussian(μ:Gaussian, σ2:Expression<Real>) -> GaussianGaussian {
  m:GaussianGaussian(μ, σ2);
  μ.setChild(m);
  return m;
}
