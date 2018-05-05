/**
 * Delayed Beta-bernoulli random variate.
 */
class DelayBetaBernoulli(x:Random<Boolean>, ρ:DelayBeta) <
    DelayValue<Boolean>(x) {
  /**
   * Success probability.
   */
  ρ:DelayBeta <- ρ;

  function doSimulate() -> Boolean {
    return simulate_beta_bernoulli(ρ.α, ρ.β);
  }
  
  function doObserve(x:Boolean) -> Real {
    return observe_beta_bernoulli(x, ρ.α, ρ.β);
  }

  function doCondition(x:Boolean) {
    (ρ.α, ρ.β) <- update_beta_bernoulli(x, ρ.α, ρ.β);
  }
}

function DelayBetaBernoulli(x:Random<Boolean>, ρ:DelayBeta) ->
    DelayBetaBernoulli {
  m:DelayBetaBernoulli(x, ρ);
  return m;
}
