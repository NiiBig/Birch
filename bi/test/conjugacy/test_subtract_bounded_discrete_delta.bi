/*
 * Test analytical solution for subtraction of bounded discrete random
 * variates.
 */
program test_subtract_bounded_discrete_delta(N:Integer <- 10000) {
  X1:Real[N,2];
  X2:Real[N,2];
 
  /* simulate forward */
  for n in 1..N {
    m:TestSubtractBoundedDiscreteDelta;
    playDelay.handle(m.simulate());
    X1[n,1..2] <- m.forward();
  }

  /* simulate backward */
  for n in 1..N {
    m:TestSubtractBoundedDiscreteDelta;
    playDelay.handle(m.simulate());
    X2[n,1..2] <- m.backward();
  }
  
  /* test result */
  if !pass(X1, X2) {
    exit(1);
  }
}

class TestSubtractBoundedDiscreteDelta < Model {
  x1:Random<Integer>;
  x2:Random<Integer>;
  s:Random<Integer>;
  
  fiber simulate() -> Event {
    x1 ~ Binomial(100, 0.75);
    x2 ~ Binomial(100, 0.25);
    s ~ Delta(x1 - x2);
  }
  
  function forward() -> Real[_] {
    y:Real[2];
    assert !x1.hasValue();
    y[1] <- x1.value();
    assert !x2.hasValue();
    y[2] <- x2.value();
    return y;
  }

  function backward() -> Real[_] {
    y:Real[2];
    s.value();
    y[2] <- x2.value();
    y[1] <- x1.value();
    return y;
  }
}
