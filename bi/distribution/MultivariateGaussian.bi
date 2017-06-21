import distribution.Delay;
import distribution.Gaussian;
import math;
import random;

/**
 * Multivariate Gaussian distribution.
 */
class MultivariateGaussian < Delay {
  /**
   * Number of dimensions.
   */
  D:Integer;

  /**
   * Value.
   */
  x:Real[_];
  
  /**
   * Weight.
   */
  w:Real;

  /**
   * Mean.
   */
  μ:Real[_];
  
  /**
   * Covariance matrix.
   */
  Σ:Real[_,_];

  function constructor(D:Integer) {
    super.constructor();
    this.D <- D;
    //this.x[D];
    //this.μ[D];
    //this.Σ[D,D];
  }

  function initialize(u:MultivariateGaussian) {
    super.initialize(u);
  }

  function initialize(μ:Real[_], Σ:Real[_,_]) {
    super.initialize();
    update(μ, Σ);
  }

  function update(μ:Real[_], Σ:Real[_,_]) {
    this.μ <- μ;
    this.Σ <- Σ;
  }

  /**
   * Value assignment.
   */
  operator <- x:Real[_] {
    set(x);
  }

  /**
   * Value conversion.
   */
  operator -> Real[_] {
    if (isMissing()) {
      graft();
      realize();
    }
    return x;
  }
  
  function set(x:Real[_]) {
    assert length(x) == D;
    
    this.x <- x;
    this.missing <- false;
  }
  
  function simulate() -> Real[_] {
    //graft();
    //realize();
    x:Real[D];
    d:Integer;
    for (d in 1..D) {
      x[d] <~ Gaussian(0.0, 1.0);
    }
    return μ + llt(Σ)*x;
  }

  function observe(x:Real[_]) -> Real {
    assert length(x) == D;
    //graft();
    //set(x);
    //realize();
    L:Real[D,D];
    L <- llt(Σ);
    return -0.5*squaredNorm(solve(L, x - μ)) - log(determinant(L)) - 0.5*Real(D)*log(2.0*π);
  }

  function doSample() {
    set(simulate());
  }
  
  function doObserve() {
    w <- observe(x);
  }
  
  function copy(o:MultivariateGaussian) {
    super.copy(o);
    x <- o.x;
    w <- o.w;
    μ <- o.μ;
    Σ <- o.Σ;
  }
}

/**
 * Create.
 */
function Gaussian(μ:Real[_], Σ:Real[_,_]) -> MultivariateGaussian {
  D:Integer <- length(μ);
  assert rows(Σ) == D;
  assert columns(Σ) == D;
  m:MultivariateGaussian(D);
  m.initialize(μ, Σ);
  return m;
}

/**
 * Initialize.
 */
//operator x:MultivariateGaussian ~ m:MultivariateGaussian {
//  assert x.isUninitialized();
//  if (!x.isMissing()) {
//    x ~> m;
//  }
//  x <- m;
//}
