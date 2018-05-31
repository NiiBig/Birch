/*
 * Delayed delta function on a linear transformation of a discrete random
 * variate.
 */
class DelayLinearDiscreteDelta(x:Random<Integer>&, a:Integer,
    μ:DelayValue<Integer>, c:Integer) < DelayValue<Integer>(x) {
  /**
   * Scale. Should be 1 or -1 to ensure integer-invertible.
   */
  a:Integer <- a;
    
  /**
   * Location.
   */
  μ:DelayValue<Integer> <- μ;

  /**
   * Offset.
   */
  c:Integer <- c;

  function simulate() -> Integer {
    return simulate_delta(a*μ.simulate() + c);
  }
  
  function observe(x:Integer) -> Real {
    return μ.observe((x - c)/a);
  }

  function pmf(x:Integer) -> Real {
    return μ.pmf((x - c)/a);
  }

  function cdf(x:Integer) -> Real {
    return μ.cdf((x - c)/a);
  }

  function lower() -> Integer? {
    l:Integer? <- μ.lower();
    if (l?) {
      return a*l! + c;
    }
  }
  
  function upper() -> Integer? {
    u:Integer? <- μ.upper();
    if (u?) {
      return a*u! + c;
    }
  }
}

function DelayLinearDiscreteDelta(x:Random<Integer>&, a:Integer,
    μ:DelayValue<Integer>, c:Integer) -> DelayLinearDiscreteDelta {
  assert abs(a) == 1;
  m:DelayLinearDiscreteDelta(x, a, μ, c);
  μ.setChild(m);
  return m;
}
