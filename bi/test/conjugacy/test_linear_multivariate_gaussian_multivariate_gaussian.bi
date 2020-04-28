/*
 * Test multivariate linear Gaussian-Gaussian conjugacy.
 */
program test_linear_multivariate_gaussian_multivariate_gaussian(
    N:Integer <- 10000) {
  m:TestLinearMultivariateGaussianMultivariateGaussian;
  test_conjugacy(m, N, 10);
}

class TestLinearMultivariateGaussianMultivariateGaussian < Model {
  μ:Random<Real[_]>;
  x:Random<Real[_]>;

  A:Real[5,5];
  μ_0:Real[5];
  Σ_0:Real[5,5];
  c:Real[5];
  Σ_1:Real[5,5];
  
  function initialize() {
    for i in 1..5 {
      μ_0[i] <- simulate_uniform(-10.0, 10.0);
      c[i] <- simulate_uniform(-10.0, 10.0);
      for j in 1..5 {
        Σ_0[i,j] <- simulate_uniform(-2.0, 2.0);
        Σ_1[i,j] <- simulate_uniform(-2.0, 2.0);
        A[i,j] <- simulate_uniform(-2.0, 2.0);
      }
    }
    Σ_0 <- Σ_0*transpose(Σ_0);
    Σ_1 <- Σ_1*transpose(Σ_1);
  }
  
  fiber simulate() -> Event {
    μ ~ Gaussian(μ_0, Σ_0);
    x ~ Gaussian(A*μ + c, Σ_1);
  }
  
  function forward() -> Real[_] {
    y:Real[10];
    y[1..5] <- μ.value();
    assert !x.hasValue();
    y[6..10] <- x.value();
    return y;
  }

  function backward() -> Real[_] {
    y:Real[10];
    y[6..10] <- x.value();
    assert !μ.hasValue();
    y[1..5] <- μ.value();
    return y;
  }
  
  function marginal() -> Distribution<Real[_]> {
    return x.distribution()!.graft();
  }
}
