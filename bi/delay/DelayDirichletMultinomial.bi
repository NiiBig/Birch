/*
 * Delayed Dirichlet-multinomial random variate.
 */
class DelayDirichletMultinomial(n:Integer, ρ:DelayDirichlet) <
    DelayValue<Integer[_]> {
  /**
   * Number of trials.
   */
  n:Integer <- n;
   
  /**
   * Category probabilities.
   */
  ρ:DelayDirichlet <- ρ;

  function simulate() -> Integer[_] {
    return simulate_dirichlet_multinomial(n, ρ.α);
  }
  
  function observe(x:Integer[_]) -> Real {
    return observe_dirichlet_multinomial(x, n, ρ.α);
  }

  function condition(x:Integer[_]) {
    ρ.α <- update_dirichlet_multinomial(x, n, ρ.α);
  }

  function pmf(x:Integer[_]) -> Real {
    return pmf_dirichlet_multinomial(x, n, ρ.α);
  }
}

function DelayDirichletMultinomial(n:Integer, ρ:DelayDirichlet) ->
    DelayDirichletMultinomial {
  m:DelayDirichletMultinomial(n, ρ);
  return m;
}
