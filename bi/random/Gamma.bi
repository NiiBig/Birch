/**
 * Gamma distribution.
 */
class Gamma(k:Expression<Real>, θ:Expression<Real>) < Random<Real> {
  /**
   * Shape.
   */
  k:Expression<Real> <- k;
  
  /**
   * Scale.
   */
  θ:Expression<Real> <- θ;

  /**
   * Updated shape.
   */
  k_p:Real;

  /**
   * Updated scale.
   */
  θ_p:Real;

  function isGamma() -> Boolean {
    return isMissing();
  }

  function getGamma() -> (Real, Real) {
    return (k_p, θ_p);
  }

  function setGamma(θ:(Real, Real)) {
    (this.k_p, this.θ_p) <- θ;
  }

  function doMarginalize() {
    k_p <- k.value();
    θ_p <- θ.value();
  }

  function doSimulate() -> Real {
    return simulate_gamma(k_p, θ_p);
  }
  
  function doObserve(x:Real) -> Real {
    return observe_gamma(x, k_p, θ_p);
  }
}

/**
 * Create gamma distribution.
 */
function Gamma(k:Expression<Real>, θ:Expression<Real>) -> Gamma {
  m:Gamma(k, θ);
  return m;
}

/**
 * Create gamma distribution.
 */
function Gamma(k:Expression<Real>, θ:Real) -> Gamma {
  return Gamma(k, Literal(θ));
}

/**
 * Create gamma distribution.
 */
function Gamma(k:Real, θ:Expression<Real>) -> Gamma {
  return Gamma(Literal(k), θ);
}

/**
 * Create gamma distribution.
 */
function Gamma(k:Real, θ:Real) -> Gamma {
  return Gamma(Literal(k), Literal(θ));
}
