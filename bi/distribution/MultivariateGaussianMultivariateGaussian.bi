/*
 * Grafted multivariate Gaussian-Gaussian distribution.
 */
final class MultivariateGaussianMultivariateGaussian(m:MultivariateGaussian,
    S:Expression<Real[_,_]>) < GraftedMultivariateGaussian(m.μ, m.Σ + S) {
  /**
   * Mean.
   */
  m:MultivariateGaussian <- m;

  /**
   * Likelihood covariance.
   */
  S:Expression<Real[_,_]> <- S;

  function update(x:Real[_]) {
    (m.μ, m.Σ) <- update_multivariate_gaussian_multivariate_gaussian(x, m.μ.value(), m.Σ.value(), S.value());
  }

  function downdate(x:Real[_]) {
    (m.μ, m.Σ) <- downdate_multivariate_gaussian_multivariate_gaussian(x, m.μ.value(), m.Σ.value(), S.value());
  }

  function graftFinalize() -> Boolean {
    S.value();
    if !m.isRealized() {
      link();
      return true;
    } else {
      return false;
    }
  }

  function link() {
    m.setChild(this);
  }
  
  function unlink() {
    m.releaseChild();
  }
}

function MultivariateGaussianMultivariateGaussian(μ:MultivariateGaussian,
    Σ:Expression<Real[_,_]>) -> MultivariateGaussianMultivariateGaussian {
  m:MultivariateGaussianMultivariateGaussian(μ, Σ);
  return m;
}
