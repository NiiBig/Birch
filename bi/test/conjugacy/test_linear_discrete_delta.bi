/*
 * Test linear-discrete-delta conjugacy.
 */
program test_linear_discrete_delta(N:Integer <- 10000) {
  X1:Real[N,2];
  X2:Real[N,2];
  
  a:Integer <- 2*simulate_uniform_int(0, 1) - 1;
  n:Integer <- simulate_uniform_int(1, 100);
  α:Real <- simulate_uniform(0.0, 10.0);
  β:Real <- simulate_uniform(0.0, 10.0);
  c:Integer <- simulate_uniform_int(0, 100);
 
  /* simulate forward */
  for auto i in 1..N {
    m:TestLinearDiscreteDelta(a, n, α, β, c);
    m.play();
    X1[i,1..2] <- m.forward();
  }

  /* simulate backward */
  for auto i in 1..N {
    m:TestLinearDiscreteDelta(a, n, α, β, c);
    m.y <- Integer(a*X1[i,2]) + c;
    m.play();
    X2[i,1..2] <- m.backward();
  }
  
  /* test result */
  if (!pass(X1, X2)) {
    exit(1);
  }
}

class TestLinearDiscreteDelta(a:Integer, n:Integer, α:Real, β:Real,
    c:Integer) < Model {
  a:Integer <- a;
  n:Integer <- n;
  α:Real <- α;
  β:Real <- β;
  c:Integer <- c;
  
  ρ:Random<Real>;
  x:Random<Integer>;
  y:Random<Integer>;
  
  fiber simulate() -> Event {
    ρ ~ Beta(α, β);
    x ~ Binomial(n, ρ);
    y ~ Delta(a*x + c);
  }
  
  function forward() -> Real[_] {
    z:Real[2];    
    z[1] <- ρ.value();
    assert !x.hasValue();
    z[2] <- x.value();
    return z;
  }

  function backward() -> Real[_] {
    z:Real[2];
    z[2] <- x.value();
    assert !ρ.hasValue();
    z[1] <- ρ.value();
    return z;
  }
}
