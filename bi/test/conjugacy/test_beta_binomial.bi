/*
 * Test beta-binomial conjugacy.
 */
program test_beta_binomial(N:Integer <- 10000) {
  X1:Real[N,2];
  X2:Real[N,2];
  n:Integer <- simulate_uniform_int(1, 100);
  α:Real <- simulate_uniform(1.0, 10.0);
  β:Real <- simulate_uniform(1.0, 10.0);
 
  /* simulate forward */
  for auto i in 1..N {
    m:TestBetaBinomial(n, α, β);
    m.play();
    X1[i,1..2] <- m.forward();
  }

  /* simulate backward */
  for auto i in 1..N {
    m:TestBetaBinomial(n, α, β);
    m.play();
    X2[i,1..2] <- m.backward();
  }
  
  /* test result */
  if (!pass(X1, X2)) {
    exit(1);
  }
}

class TestBetaBinomial(n:Integer, α:Real, β:Real) < Model {
  n:Integer <- n;
  α:Real <- α;
  β:Real <- β;
  ρ:Random<Real>;
  x:Random<Integer>;
  
  fiber simulate() -> Event {
    ρ ~ Beta(α, β);
    x ~ Binomial(n, ρ);
  }
  
  function forward() -> Real[_] {
    y:Real[2];    
    y[1] <- ρ.value();
    assert !x.hasValue();
    y[2] <- x.value();
    return y;
  }

  function backward() -> Real[_] {
    y:Real[2];    
    y[2] <- x.value();
    assert !ρ.hasValue();
    y[1] <- ρ.value();
    return y;
  }
}
