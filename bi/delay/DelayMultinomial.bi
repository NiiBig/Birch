/**
 * Multinomial random variable for delayed sampling.
 */
class DelayMultinomial(x:Random<Integer[_]>, n:Integer, ρ:Real[_]) < DelayValue<Integer[_]>(x) {
  /**
   * Number of trials.
   */
  n:Integer <- n;
   
  /**
   * Category probabilities.
   */
  ρ:Real[_] <- ρ;

  function doSimulate() -> Integer[_] {
    return simulate_multinomial(n, ρ);
  }
  
  function doObserve(x:Integer[_]) -> Real {
    return observe_multinomial(x, n, ρ);
  }
}
