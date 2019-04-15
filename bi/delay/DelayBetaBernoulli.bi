/*
 * Delayed Beta-bernoulli random variate.
 */
final class DelayBetaBernoulli(x:Random<Boolean>&, ρ:DelayBeta) <
    DelayValue<Boolean>(x) {
  /**
   * Success probability.
   */
  ρ:DelayBeta& <- ρ;

  function simulate() -> Boolean {
    return simulate_beta_bernoulli(ρ!.α, ρ!.β);
  }
  
  function observe(x:Boolean) -> Real {
    return observe_beta_bernoulli(x, ρ!.α, ρ!.β);
  }

  function update(x:Boolean) {
    (ρ!.α, ρ!.β) <- update_beta_bernoulli(x, ρ!.α, ρ!.β);
  }

  function downdate(x:Boolean) {
    (ρ!.α, ρ!.β) <- downdate_beta_bernoulli(x, ρ!.α, ρ!.β);
  }

  function pmf(x:Boolean) -> Real {
    return pmf_beta_bernoulli(x, ρ!.α, ρ!.β);
  }
}

function DelayBetaBernoulli(x:Random<Boolean>&, ρ:DelayBeta) ->
    DelayBetaBernoulli {
  m:DelayBetaBernoulli(x, ρ);
  ρ.setChild(m);
  return m;
}
