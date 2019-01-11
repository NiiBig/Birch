/**
 * Particle filter. Performs a bootstrap particle filter in the most basic
 * case, or where conjugacy relationships are used by the model, an auxiliary
 * or Rao--Blackwellized particle filter.
 */
class ParticleFilter < Sampler {
  /**
   * Particle fibers.
   */
  f:(Model,Real)![_];
  
  /**
   * Particles.
   */
  x:Model[_];
  
  /**
   * Log-weights.
   */
  w:Real[_];

  /**
   * Ancestor indices.
   */
  a:Integer[_];
  
  /**
   * Index of the chosen path at the end of the filter.
   */
  b:Integer <- 0;
  
  /**
   * For each checkpoint, the logarithm of the normalizing constant estimate
   * so far.
   */
  Z:List<Real>;
  
  /**
   * For each checkpoint, the effective sample size (ESS).
   */
  e:List<Real>;
  
  /**
   * For each checkpoint, was resampling performed?
   */
  r:List<Boolean>;
    
  /**
   * Number of particles.
   */
  nparticles:Integer <- 1;
  
  /**
   * Threshold for resampling. Resampling is performed whenever the
   * effective sample size, as a proportion of `nparticles`, drops below this
   * threshold.
   */
  trigger:Real <- 0.7;

  fiber sample(m:Model) -> (Model, Real) {
    /* if a number of checkpoints hasn't been explicitly provided, compute
     * this from the model (may still be unknown by the model, too) */
    if (!ncheckpoints?) {
      ncheckpoints <- m.checkpoints();
    }

    /* sample */  
    for (i:Integer in 1..nsamples) {
      start(m);
      if (verbose) {
        stderr.print("checkpoints:");
      }
      auto t <- 0;
      while ((!ncheckpoints? || t < ncheckpoints!) && step()) {
        t <- t + 1;
        if (verbose) {
          stderr.print(" " + t);
        }
      }
      if (ncheckpoints? && t != ncheckpoints!) {
        error("particles terminated after " + t + " checkpoints, but " + ncheckpoints! + " requested.");
      }
      if (verbose) {
        stderr.print(", log evidence: " + Z.back() + "\n");
      }
      finish();
      
      w:Real <- 0.0;
      if (!Z.empty()) {
        w <- Z.back();
      }
      yield (x[b], w);
    }
  }

  /**
   * Initialize.
   */  
  function start(m:Model) {
    auto f0 <- particle(m);
    f1:(Model,Real)![nparticles];
    x1:Model[nparticles];
    for n:Integer in 1..nparticles {
      f1[n] <- clone<(Model,Real)!>(f0);
      x1[n] <- m;
    }
    f <- f1;
    x <- x1;
    w <- vector(0.0, nparticles);
    a <- iota(1, nparticles);
    Z.clear();
    e.clear();
    r.clear();
  }
  
  /**
   * Step particles to the next checkpoint.
   */
  function step() -> Boolean {
    if (!e.empty()) {
      r.pushBack(e.back() < trigger*nparticles);
      if (r.back()) {
        resample();
        copy();
      }
    }
    auto continue <- propagate();
    if (continue) {
      reduce();
    }
    return continue;
  }
  
  /**
   * Resample particles.
   */
  function resample() {
    a <- ancestors(w);
  }

  /**
   * Copy particles after resampling.
   */
  function copy() {
    /* there are couple of different strategies here: permute_ancestors()
     * would avoid the use of the temporary f0, but f0 ensures that particles
     * with the same ancestor are contiguous in f after the copy, which is
     * more cache efficient */
    auto f0 <- f;
    for n:Integer in 1..nparticles {
      f[n] <- clone<(Model,Real)!>(f0[a[n]]);
      w[n] <- 0.0;
    }
  }
  
  /**
   * Propagate particles.
   */
  function propagate() -> Boolean {
    auto continue <- true;
    parallel for (n:Integer in 1..nparticles) {
      w1:Real;
      if f[n]? {
        (x[n], w1) <- f[n]!;
        w[n] <- w[n] + w1;
      } else {
        continue <- false;      
      }
    }
    return continue;
  }
  
  /**
   * Compute summary statistics.
   */
  function reduce() {
    /* effective sample size */
    e.pushBack(ess(w));
    if (!(e.back() > 0.0)) {  // > 0.0 as may be nan
      error("particle filter degenerated.");
    }
  
    /* normalizing constant estimate */
    auto W <- log_sum_exp(w);
    auto Z <- W - log(nparticles);
    w <- w - Z;
    if (this.Z.empty()) {
      this.Z.pushBack(Z);
    } else {
      this.Z.pushBack(this.Z.back() + Z);
    }
  }  

  /**
   * Finish the filter.
   */
  function finish() {
    b <- ancestor(w);
    if (b <= 0) {
      error("particle filter degenerated.");
    }
  }

  function read(buffer:Buffer) {
    super.read(buffer);
    auto nparticles1 <- buffer.getInteger("nparticles");
    if nparticles1? {
      nparticles <- nparticles1!;
    }
    auto trigger1 <- buffer.getReal("trigger");
    if trigger1? {
      trigger <- trigger1!;
    }
  }

  function write(buffer:Buffer) {
    super.write(buffer);
    buffer.set("nparticles", nparticles);
    buffer.set("trigger", trigger);
    buffer.set("levidence", Z);
    buffer.set("ess", e);
    buffer.set("resample", r);
  }
}

fiber particle(x:Model) -> (Model, Real) {
  auto f <- x.simulate();
  while (f?) {
    yield (x, f!);
  }
}
