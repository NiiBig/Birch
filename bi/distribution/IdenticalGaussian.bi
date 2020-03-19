/*
 * Multivariate Gaussian distribution with independent and identical
 * variance.
 */
final class IdenticalGaussian(μ:Expression<Real[_]>, σ2:Expression<Real>) <
    Distribution<Real[_]> {
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

  function simulate() -> Real[_] {
    return simulate_multivariate_gaussian(μ.value(), σ2.value());
  }
  
  function logpdf(x:Real[_]) -> Real {
    return logpdf_multivariate_gaussian(x, μ.value(), σ2.value());
  }

  function graft() -> Distribution<Real[_]> {
    if !hasValue() {
      prune();
      s1:InverseGamma?;
      m1:TransformLinearMultivariate<MultivariateNormalInverseGamma>?;
      m2:MultivariateNormalInverseGamma?;
      m3:TransformLinearMultivariate<MultivariateGaussian>?;
      m4:MultivariateGaussian?;
      r:Distribution<Real[_]>?;

      /* match a template */
      auto compare <- σ2.distribution();
      if compare? && (m1 <- μ.graftLinearMultivariateNormalInverseGamma(compare!))? {
        r <- LinearMultivariateNormalInverseGammaMultivariateGaussian(m1!.A, m1!.x, m1!.c);
      } else if compare? && (m2 <- μ.graftMultivariateNormalInverseGamma(compare!))? {
        r <- MultivariateNormalInverseGammaMultivariateGaussian(m2!);
      } else if (m3 <- μ.graftLinearMultivariateGaussian())? {
        r <- LinearMultivariateGaussianMultivariateGaussian(m3!.A, m3!.x, m3!.c, diagonal(σ2, m3!.rows()));
      } else if (m4 <- μ.graftMultivariateGaussian())? {
        r <- MultivariateGaussianMultivariateGaussian(m4!, diagonal(σ2, m4!.rows()));
      } else if (s1 <- σ2.graftInverseGamma())? {
        r <- MultivariateNormalInverseGamma(μ, Identity(μ.rows()), s1!);
      }
    
      /* finalize, and if not valid, use default template */
      if !r? || !r!.graftFinalize() {
        r <- GraftedMultivariateGaussian(μ, diagonal(σ2, μ.rows()));
        r!.graftFinalize();
      }
      return r!;
    } else {
      return this;
    }
  }

  function graftMultivariateGaussian() -> MultivariateGaussian? {
    if !hasValue() {
      prune();
      m1:TransformLinearMultivariate<MultivariateGaussian>?;
      m2:MultivariateGaussian?;
      r:MultivariateGaussian?;
    
      /* match a template */
      if (m1 <- μ.graftLinearMultivariateGaussian())? {
        r <- LinearMultivariateGaussianMultivariateGaussian(m1!.A, m1!.x, m1!.c, diagonal(σ2, m1!.c.rows()));
      } else if (m2 <- μ.graftMultivariateGaussian())? {
        r <- MultivariateGaussianMultivariateGaussian(m2!, diagonal(σ2, m2!.rows()));
      }
    
      /* finalize, and if not valid, use default template */
      if !r? || !r!.graftFinalize() {    
        r <- GraftedMultivariateGaussian(μ, diagonal(σ2, μ.rows()));
        r!.graftFinalize();
      }
      return r;
    } else {
      return nil;
    }
  }

  function graftMultivariateNormalInverseGamma(compare:Distribution<Real>) ->
      MultivariateNormalInverseGamma? {
    if !hasValue() {
      prune();
      s1:InverseGamma?;
      r:MultivariateNormalInverseGamma?;
    
      if (s1 <- σ2.graftInverseGamma())? && s1! == compare {
        r <- MultivariateNormalInverseGamma(μ, Identity(μ.rows()), s1!);
      }

      /* finalize, and if not valid, return nil */
      if !r? || !r!.graftFinalize() {
        r <- nil;
      }
      return r;
    } else {
      return nil;
    }
  }

  function graftFinalize() -> Boolean {
    assert false;  // should have been replaced during graft
    return false;
  }
}

/**
 * Create multivariate Gaussian distribution with independent and identical
 * variance.
 */
function Gaussian(μ:Expression<Real[_]>, σ2:Expression<Real>) ->
    IdenticalGaussian {
  m:IdenticalGaussian(μ, σ2);
  return m;
}

/**
 * Create multivariate Gaussian distribution with independent and identical
 * variance.
 */
function Gaussian(μ:Expression<Real[_]>, σ2:Real) -> IdenticalGaussian {
  return Gaussian(μ, Boxed(σ2));
}

/**
 * Create multivariate Gaussian distribution with independent and identical
 * variance.
 */
function Gaussian(μ:Real[_], σ2:Expression<Real>) -> IdenticalGaussian {
  return Gaussian(Boxed(μ), σ2);
}

/**
 * Create multivariate Gaussian distribution with independent and identical
 * variance.
 */
function Gaussian(μ:Real[_], σ2:Real) -> IdenticalGaussian {
  return Gaussian(Boxed(μ), Boxed(σ2));
}
