/**
 * Particle marginal importance sampler.
 */
class ParticleMarginalImportanceSampler < ForwardSampler {
  /**
   * Particle filter to use for state sampling.
   */
  filter:ParticleFilter;
  
  /**
   * Number of steps.
   */
  nsteps:Integer <- 0;

  fiber sample(model:ForwardModel) -> (ForwardModel, Real) {
    x:ForwardModel[_];
    w:Real[_];
    ess:Real;
    lweight:Real;
        
    /* subsequent samples using conditional particle filter */
    while true {
      auto f <- filter.filter(model);
      for t in 0..nsteps {
        f?;
        (x, w, ess, lweight) <- f!;
      }
      auto b <- ancestor(w);
      yield (x[b], lweight);
    }
  }

  function read(buffer:Buffer) {
    filter <-? ParticleFilter?(buffer.get("filter", filter));
    nsteps <-? buffer.get("nsteps", nsteps);
  }

  function write(buffer:Buffer) {
    buffer.set("filter", filter);
    buffer.set("nsteps", nsteps);
  }
}
