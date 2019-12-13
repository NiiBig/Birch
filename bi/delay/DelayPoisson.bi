/*
 * Delayed Poisson random variate.
 */
final class DelayPoisson(future:Integer?, futureUpdate:Boolean,
    λ:Expression<Real>) < DelayMove<DelayDiscrete>(future, futureUpdate) {
  /**
   * Rate.
   */
  λ:Expression<Real> <- λ;

  function simulate() -> Integer {
    return simulate_poisson(λ.value());
  }

  function simulatePilot() -> Integer {
    return simulate_poisson(λ.pilot());
  }

  function simulatePropose() -> Integer {
    return simulate_poisson(λ.propose());
  }

  function logpdf(x:Integer) -> Real {
    return logpdf_poisson(x, λ.value());
  }

  function lazy(x:Expression<Integer>) -> Expression<Real> {
    return lazy_poisson(x, λ);
  }

  function cdf(x:Integer) -> Real? {
    return cdf_poisson(x, λ);
  }

  function quantile(p:Real) -> Integer? {
    return quantile_poisson(p, λ);
  }

  function lower() -> Integer? {
    return 0;
  }

  function write(buffer:Buffer) {
    prune();
    buffer.set("class", "Poisson");
    buffer.set("λ", λ.value());
  }
}

function DelayPoisson(future:Integer?, futureUpdate:Boolean,
    λ:Expression<Real>) -> DelayPoisson {
  o:DelayPoisson(future, futureUpdate, λ.graft());
  return o;
}
