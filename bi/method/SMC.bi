/**
 * Sequential Monte Carlo (SMC).
 */
class SMC {
  /**
   * Simulate.
   */
  function simulate(model:String, input:Reader?, output:Writer?, T:Integer,
      N:Integer, trigger:Real) {
    f:Model![N];    // particles
    w:Real[N];      // log-weights
    a:Integer[N];   // ancestors
    Z:Real <- 0.0;  // marginal log-likelihood estimate
  
    /* initialize, running one particle to its first checkpoint, which occurs
     * after the input file has been read, then copy it around */
    f0:Model! <- particle(model, input);
    if (f0?) {
      for (n:Integer in 1..N) {
        f[n] <- f0;
        w[n] <- 0.0;
        a[n] <- n;
      }
    } else {
      stderr.print("error: particles terminated prematurely.\n");
      assert false;
    }
  
    /* filter */
    for (t:Integer in 1..T) {
    stderr.print(t + " ");
      if (ess(w) < trigger*N) {
        /* resample */
        Z <- Z + log_sum_exp(w) - log(N);
        a <- permute_ancestors(ancestors(w));
        for (n:Integer in 1..N) {
          f[n] <- f[a[n]];
          w[n] <- 0.0;
        }
      }
    
      /* propagate and weight */
      for (n:Integer in 1..N) {
        if (f[n]?) {
          w[n] <- w[n] + f[n]!.w;
        } else {
          stderr.print("error: particles terminated prematurely.\n");
          assert false;
        } 
      }
    }
    Z <- Z + log_sum_exp(w) - log(N);
    stdout.print(Z + "\n");
    
    /* output */
    if (output?) {
      f[ancestor(w)]!.output(output!.push());
    }
  }
}

fiber particle(model:String, input:Reader?) -> Model! {
  /* create model */
  x:Model? <- Model?(make(model));
  if (!x?) {
    stderr.print("error: " + model + " must be a subtype of Model with no initialization parameters.\n");
    assert false;
  }
  
  /* input */
  if (input?) {
    x!.input(input!);
  }
  yield x!;
  
  /* simulate */
  f:Real! <- x!.simulate();
  while (f?) {
    x!.w <- f!;
    yield x!;
  }
}
