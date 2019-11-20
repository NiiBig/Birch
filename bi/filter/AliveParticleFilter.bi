/**
 * Alive particle filter. When propagating and weighting particles, the
 * alive particle filter maintains $N$ particles with non-zero weight, rather
 * than $N$ particles in total as with the standard particle filter.
 */
class AliveParticleFilter {
  /**
   * Model.
   */
  model:ForwardModel;

  /**
   * Number of steps.
   */
  nsteps:Integer <- 1;

  /**
   * Number of particles.
   */
  nparticles:Integer <- 1;

  /**
   * Should delayed sampling be used?
   */
  delayed:Boolean <- true;

  fiber filter() -> (ForwardModel[_], Real[_], Real, Real) {
    auto x <- clone<ForwardModel>(model, nparticles);  // particles
    auto w <- vector(0.0, 0);  // log-weights
    auto ess <- 0.0;  // effective sample size
    auto levidence <- 0.0;  // incremental log-evidence
    
    /* event handler */
    h:Handler <- play;
    if delayed {
      h <- global.delay;
    }

    /* initialize and weight */
    parallel for n in 1..nparticles {
      w[n] <- h.handle(x[n].simulate());
    }
    (ess, levidence) <- resample_reduce(w);
    yield (x, w, ess, levidence);
   
    for t in 1..nsteps {
      /* resample */
      auto a <- resample_systematic(w);
      dynamic parallel for n in 1..nparticles {
        if a[n] != n {
          x[n] <- clone<ForwardModel>(x[a[n]]);
        }
      }

      /* propagate and weight */
      auto p <- vector(0, nparticles + 1);
      auto x0 <- x;
      auto w0 <- w;
      parallel for n in 1..nparticles + 1 {
        if n <= nparticles {
          x[n] <- clone<ForwardModel>(x0[a[n]]);
          w[n] <- h.handle(x[n].simulate(t));
          p[n] <- 1;
          while w[n] == -inf {  // repeat until weight is positive
            a[n] <- ancestor(w0);
            x[n] <- clone<ForwardModel>(x0[a[n]]);
            p[n] <- p[n] + 1;
            w[n] <- h.handle(x[n].simulate(t));
          }
        } else {
          /* propagate and weight until one further acceptance, which is
           * discarded for unbiasedness in the normalizing constant
           * estimate */
          auto w' <- 0.0;
          p[n] <- 0;
          do {
            auto a' <- ancestor(w0);
            auto x' <- clone<ForwardModel>(x0[a']);
            p[n] <- p[n] + 1;
            w' <- h.handle(x'.simulate(t));
          } while w' == -inf;  // repeat until weight is positive
        }
      }
      
      levidence <- levidence + log(nparticles) - log(sum(p) - 1);
      yield (x, w, ess, levidence);
    }
  }

  function setModel(model:ForwardModel) {
    this.model <- model;
    nsteps <- model.size();
  }

  function read(buffer:Buffer) {
    nsteps <-? buffer.get("nsteps", nsteps);
    nparticles <-? buffer.get("nparticles", nparticles);
    delayed <-? buffer.get("delayed", delayed);
  }

  function write(buffer:Buffer) {
    buffer.set("nsteps", nsteps);
    buffer.set("nparticles", nparticles);
    buffer.set("delayed", delayed);
  }
}
