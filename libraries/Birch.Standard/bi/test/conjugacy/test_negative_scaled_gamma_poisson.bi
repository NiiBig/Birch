/*
 * Test scaled gamma-Poisson conjugacy.
 */
program test_negative_scaled_gamma_poisson(N:Integer <- 10000) {
  m:TestNegativeScaledGammaPoisson;
  test_conjugacy(m, N, 2);
}

class TestNegativeScaledGammaPoisson < Model {
  λ:Random<Real>;
  x:Random<Integer>;

  a:Real;
  k:Real;
  θ:Real;
  
  function initialize() {
    a <- simulate_uniform(1.0, 10.0);
    k <- simulate_uniform_int(1, 10);
    θ <- simulate_uniform(0.0, 10.0);
  }
  
  fiber simulate() -> Event {
    λ ~ Gamma(k, θ);
    x ~ Poisson(λ/a);
  }
  
  function forward() -> Real[_] {
    y:Real[2];
    y[1] <- λ.value();
    assert !x.hasValue();
    y[2] <- x.value();
    return y;
  }

  function backward() -> Real[_] {
    y:Real[2];
    y[2] <- x.value();
    assert !λ.hasValue();
    y[1] <- λ.value();
    return y;
  }
  
  function marginal() -> Distribution<Integer> {
    return x.distribution()!.graft();
  }
}
