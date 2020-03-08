/**
 * Weibull distribution.
 */
final class Weibull(k:Expression<Real>, λ:Expression<Real>) <
    Distribution<Real> {
  /**
   * Shape.
   */
  k:Expression<Real> <- k;

  /**
   * Scale.
   */
  λ:Expression<Real> <- λ;

  function simulate() -> Real {
    return simulate_weibull(k.value(), λ.value());
  }
  
  function logpdf(x:Real) -> Real {
    return logpdf_weibull(x, k.value(), λ.value());
  }

  function cdf(x:Real) -> Real? {
    return cdf_weibull(x, k.value(), λ.value());
  }

  function quantile(P:Real) -> Real? {
    return quantile_weibull(P, k.value(), λ.value());
  }

  function lower() -> Real? {
    return 0.0;
  }

  function graft() -> Distribution<Real> {
    prune();
    return this;
  }

  function write(buffer:Buffer) {
    prune();
    buffer.set("class", "Weibull");
    buffer.set("k", k);
    buffer.set("λ", λ);
  }
}

/**
 * Create Weibull distribution.
 */
function Weibull(k:Expression<Real>, λ:Expression<Real>) -> Weibull {
  m:Weibull(k, λ);
  return m;
}

/**
 * Create Weibull distribution.
 */
function Weibull(k:Expression<Real>, λ:Real) -> Weibull {
  return Weibull(k, Boxed(λ));
}

/**
 * Create Weibull distribution.
 */
function Weibull(k:Real, λ:Expression<Real>) -> Weibull {
  return Weibull(Boxed(k), λ);
}

/**
 * Create Weibull distribution.
 */
function Weibull(k:Real, λ:Real) -> Weibull {
  return Weibull(Boxed(k), Boxed(λ));
}
