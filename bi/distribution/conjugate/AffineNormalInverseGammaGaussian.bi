/*
 * Gaussian with normal inverse-gamma prior on mean and variance,
 * where the normal on the mean is further modified with an affine
 * transformation.
 */
class AffineNormalInverseGammaGaussian < Random<Real> {
  /**
   * Scale.
   */
  a:Real;

  /**
   * Random variable.
   */
  x:NormalInverseGamma;
  
  /**
   * Offset.
   */
  c:Real;

  /**
   * Variance.
   */
  σ2:InverseGamma;

  function initialize(a:Real, x:NormalInverseGamma, c:Real, σ2:InverseGamma) {
    //assert x.σ2 == σ2;
    super.initialize(x);
    this.a <- a;
    this.x <- x;
    this.c <- c;
    this.σ2 <- σ2;
  }
  
  function doCondition() {
    μ_1:Real;
    a2_1:Real;
    α_1:Real;
    β_1:Real;
    
    if (x.isRealized() && !σ2.isRealized()) {
      (α_1, β_1) <- update_inverse_gamma_gaussian(value(), a*x.value() + c, σ2.α, σ2.β);
      σ2.update(α_1, β_1);
    } else if (!x.isRealized() && σ2.isRealized()) {
      μ_1 <- x.μ;
      a2_1 <- x.a2*σ2.value();
      (μ_1, a2_1) <- update_affine_gaussian_gaussian(value(), a, μ_1, a2_1, a*μ_1 + c, a*a*a2_1 + σ2.value());
      x.update(μ_1, a2_1);
    } else {
      (μ_1, a2_1, α_1, β_1) <- update_affine_normal_inverse_gamma_gaussian(a, value(), c, x.μ, x.a2, σ2.α, σ2.β);
      x.update(μ_1, a2_1);
      σ2.update(α_1, β_1);
    }
  }

  function doRealize() {
    μ_1:Real;
    σ2_1:Real;
    
    if (x.isRealized() && σ2.isRealized()) {
      μ_1 <- a*x.value() + c;
      σ2_1 <- σ2.value();
      if (isMissing()) {
        set(simulate_gaussian(μ_1, σ2_1));
      } else {
        setWeight(observe_gaussian(value(), μ_1, σ2));
      }
    } else if (x.isRealized() && !σ2.isRealized()) {
      μ_1 <- a*x.value() + c;
      if (isMissing()) {
        set(simulate_inverse_gamma_gaussian(μ_1, σ2.α, σ2.β));
      } else {
        setWeight(observe_inverse_gamma_gaussian(value(), μ_1, σ2.α, σ2.β));
      }
    } else if (!x.isRealized() && σ2.isRealized()) {
      μ_1 <- a*x.μ + c;
      σ2_1 <- (a*a*x.a2 + 1.0)*σ2.value();
      if (isMissing()) {
        set(simulate_gaussian(μ_1, σ2_1));
      } else {
        setWeight(observe_gaussian(value(), μ_1, σ2_1));
      }
    } else {
      if (isMissing()) {
        set(simulate_affine_normal_inverse_gamma_gaussian(a, x.μ, c,
            x.a2, σ2.α, σ2.β));
      } else {
        setWeight(observe_affine_normal_inverse_gamma_gaussian(value(),
            a, x.μ, c, x.a2, σ2.α, σ2.β));
      }
    }
  }
}

/**
 * Create Gaussian distribution.
 */
function Gaussian(μ:AffineExpression, σ2:InverseGamma) -> Random<Real> {
  x:NormalInverseGamma? <- NormalInverseGamma?(μ.x);
  if (x?) {  // and σ2 match
    y:AffineNormalInverseGammaGaussian;
    y.initialize(μ.a, x!, μ.c, σ2);
    return y;
  } else {
    return Gaussian(μ.value(), σ2);
  }
}

/**
 * Create Gaussian distribution.
 */
function Gaussian(μ:AffineExpression, σ2:Random<Real>) -> Random<Real> {
  σ2_1:InverseGamma? <- InverseGamma?(σ2);
  if (σ2_1?) {
    return Gaussian(μ, σ2_1!);
  } else {
    return Gaussian(μ, σ2.value());
  }
}
