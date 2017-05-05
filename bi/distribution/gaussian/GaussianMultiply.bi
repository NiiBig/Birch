import distribution.Gaussian;

/**
 * Variate that is scalar multiple of a Gaussian variate.
 */
class GaussianMultiply < Gaussian {
  /**
   * Scalar operand.
   */
  a:Real;
  
  /**
   * Gaussian operand.
   */
  u:Gaussian;

  function create(a:Real, u:Gaussian) {
    initialise(u);
    this.a <- a;
    this.u <- u;
  }
  
  function doMarginalise() {
    if (u.isRealised()) {
      this.x <- a*u.x;
      realise();
    } else {
      this.μ <- a*u.μ;
      this.σ <- abs(a*u.σ);
    }
  }
  
  function doRealise() {
    if (u.isTerminal()) {
      u.set(x/a);
    }
  }
}

function a:Real*u:Gaussian -> Gaussian {
  v:GaussianMultiply;
  v.create(a, u);
  return v;
}

function u:Gaussian*a:Real -> Gaussian {
  v:GaussianMultiply;
  v.create(a, u);
  return v;
}
