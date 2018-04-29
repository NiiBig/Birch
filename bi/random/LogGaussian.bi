/**
 * Log-Gaussian distribution.
 */
class LogGaussian(μ:Expression<Real>, σ2:Expression<Real>) < Random<Real> {
  /**
   * Mean after log transformation.
   */
  μ:Expression<Real> <- μ;
  
  /**
   * Variance after log transformation.
   */
  σ2:Expression<Real> <- σ2;

  function doGraft() -> Delay? {
    s2:DelayInverseGamma?;
    m1:DelayAffineNormalInverseGamma?;
    m2:DelayNormalInverseGamma?;
    m3:TransformAffineGaussian?;
    m4:DelayGaussian?;
        
    if (s2 <- σ2.graftInverseGamma())? {
      if (m1 <- μ.graftAffineNormalInverseGamma(σ2))? {
        return DelayAffineNormalInverseGammaLogGaussian(this, m1!);
      } else if (m2 <- μ.graftNormalInverseGamma(σ2))? {
        return DelayNormalInverseGammaLogGaussian(this, m2!);
      } else {
        return DelayInverseGammaLogGaussian(this, μ, σ2!);
      }
    } else if (m3 <- μ.graftAffineGaussian())? {
      return DelayAffineLogGaussian(this, m3!, σ2);
    } else if (m4 <- μ.graftGaussian())? {
      return DelayGaussianLogGaussian(this, m4!, σ2);
    } else {
      return DelayLogGaussian(this, μ, σ2);
    }
  }
}

/**
 * Create log-Gaussian distribution.
 */
function LogGaussian(μ:Expression<Real>, σ2:Expression<Real>) -> LogGaussian {
  m:LogGaussian(μ, σ2);
  return m;
}

/**
 * Create log-Gaussian distribution.
 */
function LogGaussian(μ:Expression<Real>, σ2:Real) -> LogGaussian {
  return LogGaussian(μ, Boxed(σ2));
}

/**
 * Create log-Gaussian distribution.
 */
function LogGaussian(μ:Real, σ2:Expression<Real>) -> LogGaussian {
  return LogGaussian(Boxed(μ), σ2);
}

/**
 * Create log-Gaussian distribution.
 */
function LogGaussian(μ:Real, σ2:Real) -> LogGaussian {
  return LogGaussian(Boxed(μ), Boxed(σ2));
}
