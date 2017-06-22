import distribution.MultivariateGaussian;
import distribution.multivariate_gaussian.AffineMultivariateGaussianExpression;
import math;

/**
 * Multivariate Gaussian that has a mean which is an affine transformation of
 * another multivariate Gaussian.
 *
 * `R` Number of rows in transformation.
 * `C` Number of columns in transformation.
 */
class AffineMultivariateGaussian(R:Integer, C:Integer) <
    MultivariateGaussian(R) {
  /**
   * Matrix of affine transformation.
   */
  A:Real[R,C];

  /**
   * Mean.
   */
  μ:MultivariateGaussian;
  
  /**
   * Vector of affine transformation.
   */
  c:Real[R];

  /**
   * Disturbance covariance.
   */
  Q:Real[R,R];
  
  /**
   * Marginalized prior mean.
   */
  y:Real[R];
  
  /**
   * Marginalized prior covariance.
   */
  S:Real[R,R];

  function initialize(A:Real[_,_], μ:MultivariateGaussian, c:Real[_],
      Q:Real[_,_]) {
    super.initialize(μ);
    this.A <- A;
    this.μ <- μ;
    this.c <- c;
    this.Q <- Q;
  }
  
  function doMarginalize() {
    y <- A*μ.μ + c;
    S <- A*μ.Σ*transpose(A) + Q;
    update(y, S);
  }

  function doForward() {
    y <- A*μ.x + c;
    S <- Q;
    update(y, S);
  }
  
  function doCondition() {
    K:Real[C,R];
    K <- μ.Σ*transpose(A)*inverse(S);
    μ.update(μ.μ + K*(x - y), μ.Σ - K*A*μ.Σ);
  }

  function copy(o:AffineMultivariateGaussian) {
    super.copy(o);
    A <- o.A;
    μ.copy(o.μ);
    c <- o.c;
    Q <- o.Q;
    y <- o.y;
    S <- o.S;
    
    /* update graph edges */
    setParent(μ);
    if (isMarginalized()) {
      μ.setChild(this);
    }
  }
}

function Gaussian(μ:MultivariateGaussian, Q:Real[_,_]) ->
    MultivariateGaussian {
  v:AffineMultivariateGaussian(μ.D, μ.D);
  v.initialize(identity(μ.D, μ.D), μ, vector(0.0, μ.D), Q);
  return v;
}

function Gaussian(μ:AffineMultivariateGaussianExpression, Q:Real[_,_]) ->
    MultivariateGaussian {
  v:AffineMultivariateGaussian(μ.R, μ.C);
  v.initialize(μ.A, μ.u, μ.c, Q);
  return v;
}
