/**
 * Multivariate Gaussian distribution with independent and identical
 * variance.
 */
final class IdenticalGaussian(future:Real[_]?, futureUpdate:Boolean,
    μ:Expression<Real[_]>, σ2:Expression<Real>) <
    Distribution<Real[_]>(future, futureUpdate) {
  /**
   * Mean.
   */
  μ:Expression<Real[_]> <- μ;
  
  /**
   * Variance.
   */
  σ2:Expression<Real> <- σ2;

  function rows() -> Integer {
    return μ.rows();
  }

  function graft() -> Distribution<Real[_]> {
    prune();
    s1:InverseGamma?;
    m1:TransformLinearMultivariate<MultivariateNormalInverseGamma>?;
    m2:MultivariateNormalInverseGamma?;
    m3:TransformLinearMultivariate<MultivariateGaussian>?;
    m4:MultivariateGaussian?;

    if (m1 <- μ.graftLinearMultivariateNormalInverseGamma())? &&
        m1!.x.σ2 == σ2 {
      return LinearMultivariateNormalInverseGammaMultivariateGaussian(
          future, futureUpdate, m1!.A, m1!.x, m1!.c);
    } else if (m2 <- μ.graftMultivariateNormalInverseGamma())? &&
      m2!.σ2 == σ2 {
      return MultivariateNormalInverseGammaMultivariateGaussian(
          future, futureUpdate, m2!);
    } else if (m3 <- μ.graftLinearMultivariateGaussian())? {
      return LinearMultivariateGaussianMultivariateGaussian(future,
          futureUpdate, m3!.A, m3!.x, m3!.c, diagonal(σ2.value(),
          m3!.rows()));
    } else if (m4 <- μ.graftMultivariateGaussian())? {
      return MultivariateGaussianMultivariateGaussian(future,
          futureUpdate, m4!, diagonal(σ2, m4!.rows()));
    } else if (s1 <- σ2.graftInverseGamma())? {
      return MultivariateNormalInverseGamma(future, futureUpdate, μ,
          identity(μ.rows()), s1!);
    } else {
      return MultivariateGaussian(future, futureUpdate, μ,
          diagonal(σ2, μ.rows()));
    }
  }

  function graftMultivariateGaussian() -> MultivariateGaussian? {
    prune();
    m1:TransformLinearMultivariate<MultivariateGaussian>?;
    m2:MultivariateGaussian?;
    if (m1 <- μ.graftLinearMultivariateGaussian())? {
      return LinearMultivariateGaussianMultivariateGaussian(future,
          futureUpdate, m1!.A, m1!.x, m1!.c, diagonal(σ2, length(m1!.c)));
    } else if (m2 <- μ.graftMultivariateGaussian())? {
      return MultivariateGaussianMultivariateGaussian(future,
          futureUpdate, m2!, diagonal(σ2, m2!.rows()));
    } else {
      return MultivariateGaussian(future, futureUpdate, μ,
          diagonal(σ2, μ.rows()));
    }
  }

  function graftMultivariateNormalInverseGamma() -> MultivariateNormalInverseGamma? {
    prune();
    s1:InverseGamma?;
    if (s1 <- σ2.graftInverseGamma())? {
      return MultivariateNormalInverseGamma(future, futureUpdate, μ,
          identity(μ.rows()), s1!);
    }
    return nil;
  }
}

/**
 * Create multivariate Gaussian distribution.
 */
function Gaussian(μ:Expression<Real[_]>, σ2:Expression<Real>) ->
    IdenticalGaussian {
  m:IdenticalGaussian(nil, true, μ, σ2);
  return m;
}

/**
 * Create multivariate Gaussian distribution.
 */
function Gaussian(μ:Expression<Real[_]>, σ2:Real) -> IdenticalGaussian {
  return Gaussian(μ, Boxed(σ2));
}

/**
 * Create multivariate Gaussian distribution.
 */
function Gaussian(μ:Real[_], σ2:Expression<Real>) -> IdenticalGaussian {
  return Gaussian(Boxed(μ), σ2);
}

/**
 * Create multivariate Gaussian distribution.
 */
function Gaussian(μ:Real[_], σ2:Real) -> IdenticalGaussian {
  return Gaussian(Boxed(μ), Boxed(σ2));
}
