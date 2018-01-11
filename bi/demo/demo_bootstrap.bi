/**
 * Demonstrates a simple bootstrap particle filter over a linear-Gaussian
 * state-space model.
 *
 *   - `-N` : Number of particles.
 *   - `-T` : Number of time steps.
 */
program demo_bootstrap(N:Integer <- 10, T:Integer <- 10) {  
  x:Real![N];     // particles
  w:Real[N];      // log-weights
  a:Integer[N];   // ancestor indices
  W:Real <- 0.0;  // marginal likelihood

  /* initialize */
  for (n:Integer in 1..N) {
    x[n] <- run(T);
    if (x[n]?) {
      w[n] <- x[n]!;
    } else {
      w[n] <- -inf;
    }
  }
  W <- log_sum_exp(w) - log(Real(N));

  for (t:Integer in 2..T) {
    stdout.print(t + "\n");
    
    /* resample */
    a <- ancestors(w);
    for (n:Integer in 1..N) {
      if (a[n] != n) {
        x[n] <- x[a[n]];
      }
    }
    
    /* propagate and weight */
    for (n:Integer in 1..N) {
      if (x[n]?) {
        w[n] <- x[n]!;
      } else {
        w[n] <- -inf;
      }
    }
    
    /* marginal log-likelihood estimate */
    W <- W + log_sum_exp(w) - log(Real(N));
  }
    
  /* output */
  stdout.print(N + " " + W + "\n");
}


class BootstrapExample(T:Integer) {
  a:Real <- 0.8;
  σ2:Real <- 1.0;
  x:Random<Real>[T];
  y:Random<Real>[T];

  fiber simulate() -> Real! {
    x[1] <~ Gaussian(0.0, σ2);
    y[1] ~> Gaussian(x[1], σ2);
    for (t:Integer in 2..T) {
      x[t] <~ Gaussian(a*x[t-1], σ2);
      y[t] ~> Gaussian(x[t], σ2);
    }
  }

  function input() {
    input:InputStream;
    input.open("data/y.csv");    
    for (t:Integer in 1..T) {
      y[t] <- input.readReal();
    }
    input.close();
  }
}

closed fiber run(T:Integer) -> Real! {
  x:BootstrapExample(T);
  x.input();
  x.simulate();
}

class TestTest {
  x:Random<Integer>;
  
  fiber f() -> Boolean! {
    yield !x.isMissing();
    x <- 1;
    while (true) {
      yield !x.isMissing();
    }
  }
}
