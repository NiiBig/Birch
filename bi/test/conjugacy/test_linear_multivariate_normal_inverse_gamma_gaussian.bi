/*
 * Test multivariate linear normal-inverse-gamma-Gaussian conjugacy.
 */
program test_linear_multivariate_normal_inverse_gamma_gaussian(
    N:Integer <- 10000) {
  m:TestLinearMultivariateNormalInverseGammaGaussian;
  test_conjugacy(m, N, 12);
}

class TestLinearMultivariateNormalInverseGammaGaussian < Model {
  σ2:Random<Real>;
  μ:Random<Real[_]>;
  x:Random<Real>;

  a:Real[10];
  μ_0:Real[10];
  Σ:Real[10,10];
  c:Real;
  α:Real;
  β:Real;
  
  function initialize() {
    α <- simulate_uniform(2.0, 10.0);
    β <- simulate_uniform(0.0, 10.0);
    for i in 1..10 {
      a[i] <- simulate_uniform(-2.0, 2.0);
      μ_0[i] <- simulate_uniform(-10.0, 10.0);
      for j in 1..10 {
        Σ[i,j] <- simulate_uniform(-2.0, 2.0);
      }
    }
    c <- simulate_uniform(-10.0, 10.0);
    Σ <- Σ*transpose(Σ);
  }
  
  fiber simulate() -> Event {
    σ2 ~ InverseGamma(α, β);
    μ ~ Gaussian(μ_0, Σ, σ2);
    x ~ Gaussian(dot(a, μ) + c, σ2);
  }
  
  function forward() -> Real[_] {
    y:Real[12];
    y[1] <- σ2.value();
    assert !μ.hasValue();
    y[2..11] <- μ.value();
    assert !x.hasValue();
    y[12] <- x.value();
    return y;
  }

  function backward() -> Real[_] {
    y:Real[12];
    y[12] <- x.value();
    assert !μ.hasValue();
    y[2..11] <- μ.value();
    assert !σ2.hasValue();
    y[1] <- σ2.value();
    return y;
  }
  
  function marginal() -> Distribution<Real> {
    return x.distribution()!.graft();
  }
}
