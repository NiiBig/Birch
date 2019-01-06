/**
 * Alive particle filter. When propagating and weighting particles, the
 * alive particle filter maintains $N$ particles with non-zero weight, rather
 * than $N$ particles in total as with the standard particle filter.
 */
class AliveParticleFilter < ParticleFilter {
  /**
   * For each checkpoint, the number of propagations that were performed to
   * achieve $N$ acceptances.
   */
  propagations:List<Integer>;

  function initialize(m:Model) {
    super.initialize(m);
    propagations.clear();
    trigger <- 1.0;  // always resample
  }

  function propagate() -> Boolean {  
    /* diagnostics */    
    auto f0 <- f;
    auto w0 <- w;
    auto P <- 0;  // number of propagations
    
    /* propagate and weight until nparticles acceptances; the first
     * nparticles proposals are drawn using the standard resampler, then each
     * is propagated until it has non-zero weight, proposal alternatives with
     * a categorical draw */  
    auto continue <- true;
    cpp {{
    /* use an atomic to accumulate the number of propagations within the
     * parallel loop */
    std::atomic<int> P;
    P = 0;
    }}
    parallel for (n:Integer in 1..nparticles) {
      do {
        f[n] <- clone<(Model,Real)!>(f0[a[n]]);
        if (f[n]?) {
          cpp {{
          ++P;
          }}
          result:Model?;
          (result, w[n]) <- f[n]!;
          if (w[n] == -inf) {
            a[n] <- ancestor(w0);
          }
        } else {
          continue <- false;
        }
      } while (continue && w[n] == -inf);
    }
    cpp {{
    P_ = P;
    // can use the normal counter henceforth
    }}
    
    if (continue) {
      /* propagate and weight until one further acceptance, that is discarded
       * for unbiasedness in the normalizing constant estimate */
      f1:(Model, Real)!;
      s1:Model?;
      w1:Real;
      do {
        f1 <- clone<(Model,Real)!>(f0[ancestor(w0)]);
        if (f1?) {
          P <- P + 1;
          (s1, w1) <- f1!;
        } else {
          continue <- false;
        }
      } while (continue && w1 == -inf);
    }
    propagations.pushBack(P);
    return continue;
  }
  
  function reduce() {
    /* effective sample size */
    ess.pushBack(global.ess(w));
    if (!(ess.back() > 0.0)) {  // may be nan
      error("particle filter degenerated.");
    }
  
    /* normalizing constant estimate */
    auto W <- log_sum_exp(w);
    auto P <- propagations.back();
    w <- w - (W - log(P - 1));
    Z <- Z + W - log(P - 1);
    evidence.pushBack(Z);
  }

  function write(buffer:Buffer) {
    super.write(buffer);
    buffer.set("propagations", propagations);
  }
}
