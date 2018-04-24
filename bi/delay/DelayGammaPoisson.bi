/**
 * Gamma-Poisson random variable for delayed sampling.
 */
class DelayGammaPoisson(x:Random<Integer>, λ:DelayGamma) < DelayValue<Integer>(x) {
  /**
   * Rate.
   */
  λ:DelayGamma <- λ;

  function doSimulate() -> Integer {
    return simulate_gamma_poisson(λ.k, λ.θ);
  }
  
  function doObserve(x:Integer) -> Real {
    return observe_gamma_poisson(x, λ.k, λ.θ);
  }

  function doCondition(x:Integer) {
    (λ.k, λ.θ) <- update_gamma_poisson(x, λ.k, λ.θ);
  }
}
