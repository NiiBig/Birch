/**
 * Particle filter.
 *
 * Configuration file
 * ------------------
 *
 * The following options are supported in the method configuration file.
 *
 *  - `nparticles`: Number of particles to use.
 *
 *  - `trigger`: Threshold for resampling. Resampling is performed
 *     whenever the effective sample size, as a proportion of `--nparticles`,
 *     drops below this threshold.
 */
class ParticleFilter < Method {
  /**
   * Particles.
   */
  f:Variate![_];
  
  /**
   * Log-weights.
   */
  w:Real[_];
  
  /**
   * For each checkpoint, the effective sample size (ESS).
   */
  e:Real[_];
  
  /**
   * For each checkpoint, was resampling performed?
   */
  r:Boolean[_];
  
  /**
   * For each checkpoint, the normalizing constant estimate up to that
   * checkpoint.
   */
  Z:Real[_];
  
  /**
   * Number of particles.
   */
  N:Integer <- 1;
  
  /**
   * Relative ESS below which resampling should be triggered.
   */
  trigger:Real <- 0.7;

  function sample(v:Variate, m:Model, ncheckpoints:Integer, verbose:Boolean) -> Variate {
    start(v, m, ncheckpoints);
    for (t:Integer in 1..ncheckpoints) {
      if (ncheckpoints > 1 && verbose) {
        stderr.print(t + " ");
      }
      step(t);
    }
    if (verbose) {
      stderr.print(Z[ncheckpoints] + "\n");
    }
    finish();
    
    /* choose sample */
    auto b <- ancestor(w);
    if (b > 0) {
      f[b]!.w <- Z[ncheckpoints];
      return f[b]!;
    } else {
      stderr.print("error: particle filter degenerated.\n");
      exit(1);
    }
  }

  /**
   * Start the filter.
   */  
  function start(v:Variate, m:Model, ncheckpoints:Integer) {
    f0:Variate! <- particle(v, m);
    f1:Variate![N];
    for n:Integer in 1..N {
      f1[n] <- clone(f0);
    }
    
    this.f <- f1;
    this.w <- vector(0.0, N);
    this.e <- vector(0.0, ncheckpoints);
    this.r <- vector(false, ncheckpoints);
    this.Z <- vector(0.0, ncheckpoints);
  }
  
  /**
   * Step to the next checkpoint.
   *
   * - t: Checkpoint number.
   */
  function step(t:Integer) {
    /* resample (if necessary) */
    e[t] <- ess(w);
    if (!(e[t] > 0.0)) {  // may be nan
      stderr.print("error: particle filter degenerated.\n");
      exit(1);
    }
    r[t] <- e[t] < trigger*N;
    if (r[t]) {
      auto a <- ancestors(w);
      auto g <- f;
      for (n:Integer in 1..N) {
        f[n] <- clone(g[a[n]]);
        w[n] <- 0.0;
      }
    }

    /* propagate and weight */
    parallel for (n:Integer in 1..N) {
      if (f[n]?) {
        w[n] <- w[n] + f[n]!.w;
      } else {
        stderr.print("error: particles terminated prematurely.\n");
        exit(1);
      }
    }
    
    /* update normalizing constant estimate */
    auto W <- log_sum_exp(w);
    w <- w - (W - log(N));
    if (t > 1) {
      Z[t] <- Z[t - 1] + (W - log(N));
    } else {
      Z[t] <- W - log(N);
    }
  }
  
  /**
   * Finish the filter.
   */
  function finish() {
    //
  }
  
  function read(reader:Reader) {
    auto a <- reader.getInteger("nparticles");
    if (a?) {
      N <- a!;
    }
    auto b <- reader.getReal("trigger");
    if (b?) {
      trigger <- b!;
    }
  }

  function write(writer:Writer) {
    writer.setInteger("nparticles", N);
    writer.setReal("trigger", trigger);
    writer.setRealVector("ess", e);
    writer.setBooleanVector("resample", r);
    writer.setRealVector("evidence", Z);
  }
}

/*
 * Particle.
 */
fiber particle(v:Variate, m:Model) -> Variate {
  auto f <- m.simulate(v);
  while (f?) {
    v.w <- f!;
    yield v;
  }
}
