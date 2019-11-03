/*
 * Test scaled gamma-Poisson conjugacy.
 */
program test_scaled_gamma_poisson(N:Integer <- 10000) {
  X1:Real[N,2];
  X2:Real[N,2];
  a:Real <- simulate_uniform(0.0, 100.0);
  k:Real <- simulate_uniform_int(1, 10);
  θ:Real <- simulate_uniform(0.0, 10.0);
 
  /* simulate forward */
  for auto n in 1..N {
    m:TestScaledGammaPoisson(a, k, θ);
    m.play();
    X1[n,1..2] <- m.forward();
  }

  /* simulate backward */
  for auto n in 1..N {
    m:TestScaledGammaPoisson(a, k, θ);
    m.play();
    X2[n,1..2] <- m.backward();
  }
  
  /* test result */
  if (!pass(X1, X2)) {
    exit(1);
  }
}

class TestScaledGammaPoisson(a:Real, k:Real, θ:Real) < Model {
  a:Real <- a;
  k:Real <- k;
  θ:Real <- θ;
  λ:Random<Real>;
  x:Random<Integer>;
  
  fiber simulate() -> Event {
    λ ~ Gamma(k, θ);
    x ~ Poisson(a*λ);
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
}
