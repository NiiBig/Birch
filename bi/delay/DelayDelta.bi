/*
 * Delayed delta random variate.
 */
class DelayDelta(x:Random<Integer>&, μ:Integer) < DelayValue<Integer>(x) {
  /**
   * Location.
   */
  μ:Integer <- μ;

  function simulate() -> Integer {
    return simulate_delta(μ);
  }
  
  function observe(x:Integer) -> Real {
    return observe_delta(x, μ);
  }

  function pmf(x:Integer) -> Real {
    return pmf_delta(x, μ);
  }
}

function DelayDelta(x:Random<Integer>&, μ:Integer) -> DelayDelta {
  m:DelayDelta(x, μ);
  return m;
}
