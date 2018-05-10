/*
 * Delayed Bernoulli random variate.
 */
class DelayBernoulli(x:Random<Boolean>&, ρ:Real) < DelayValue<Boolean>(x) {
  /**
   * Success probability.
   */
  ρ:Real <- ρ;

  function simulate() -> Boolean {
    return simulate_bernoulli(ρ);
  }
  
  function observe(x:Boolean) -> Real {
    return observe_bernoulli(x, ρ);
  }

  function pmf(x:Boolean) -> Real {
    return pmf_bernoulli(x, ρ);
  }
}

function DelayBernoulli(x:Random<Boolean>&, ρ:Real) -> DelayBernoulli {
  m:DelayBernoulli(x, ρ);
  return m;
}
