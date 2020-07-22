/**
 * Delta function on a linear transformation of a discrete random
 * variate.
 */
final class LinearDiscrete(a:Expression<Integer>, μ:Discrete,
    c:Expression<Integer>) < Discrete {
  /**
   * Scale. Should be 1 or -1 to ensure integer-invertible.
   */
  a:Expression<Integer> <- a;
    
  /**
   * Location.
   */
  μ:Discrete <- μ;

  /**
   * Offset.
   */
  c:Expression<Integer> <- c;

  function supportsLazy() -> Boolean {
    return false;
  }

  function simulate() -> Integer {
    if value? {
      return simulate_delta(value!);
    } else {
      return simulate_delta(a.value()*μ.simulate() + c.value());
    }
  }

//  function simulateLazy() -> Integer? {
//    if value? {
//      return simulate_delta(value!);
//    } else {
//      return simulate_delta(a.get()*μ.simulateLazy()! + c.get());
//    }
//  }
  
  function logpdf(x:Integer) -> Real {
    if value? {
      return logpdf_delta(x, value!);
    } else {
      return μ.logpdf((x - c.value())/a.value()) - log(abs(Real(a.value())));
    }
  }

//  function logpdfLazy(x:Expression<Integer>) -> Expression<Real>? {
//    if value? {
//      return logpdf_lazy_delta(x, value!);
//    } else {
//      return μ.logpdfLazy((x - c)/a) - log(abs(Real(a)));
//    }
//  }
  
  function update(x:Integer) {
    μ.clamp((x - c.value())/a.value());
  }

//  function updateLazy(x:Expression<Integer>) {
//
//  }

  function cdf(x:Integer) -> Real? {
    return μ.cdf((x - c.value())/a.value());
  }

  function lower() -> Integer? {
    auto l <- μ.lower();
    if l? {
      l <- a.value()*l! + c.value();
    }
    return l;
  }
  
  function upper() -> Integer? {
    auto u <- μ.upper();
    if u? {
      u <- a.value()*u! + c.value();
    }
    return u;
  }

  function link() {
    // clamp() used instead for discrete enumerations
    //μ.setChild(this);
  }
  
  function unlink() {
    // clamp() used instead for discrete enumerations
    //μ.releaseChild(this);
  }
}

function LinearDiscrete(a:Expression<Integer>, μ:Discrete,
    c:Expression<Integer>) -> LinearDiscrete {
  m:LinearDiscrete(a, μ, c);
  m.link();
  return m;
}
