class Multi < StateSpaceModel<Global,List<Track>,List<Random<Real[_]>>> {
  /**
   * Current time.
   */
  t:Integer <- 0;

  /**
   * All tracks up to current time.
   */
  z:List<Track>;
  
  fiber transition(x':List<Track>, x:List<Track>, θ:Global) -> Event {
    /* update time */
    t <- t + 1;

    /* move current objects */
    auto track <- x.walk();
    while track? {
      ρ:Real <- pmf_poisson(t - track!.t - 1, θ.τ);
      R:Real <- 1.0 - cdf_poisson(t - track!.t - 1, θ.τ) + ρ;
      s:Boolean;
      s <~ Bernoulli(1.0 - ρ/R);  // does the object survive?
      if s {
        track!.step();
        x'.pushBack(track!);
      }
    }
    
    /* birth new objects */
    N:Integer;
    N <~ Poisson(θ.λ);
    for n:Integer in 1..N {
      track:Track;
      track.t <- t;
      track.θ <- θ;
      track.start();  // up to parameters
      track.step();   // up to initial time
      x'.pushBack(track);
      z.pushBack(track);
    }
  }

  fiber observation(y:List<Random<Real[_]>>, x:List<Track>, θ:Global) -> Event {
    if !y.empty() {
      association(y, x, θ);
    } else {
      /* clutter */
      N:Integer;
      N <~ Poisson(θ.μ);
      for n:Integer in 1..(N + 1) {
        clutter:Random<Real[_]>;
        clutter <~ Uniform(θ.l, θ.u);
        y.pushBack(clutter);
      }
    }
  }

  fiber association(y:List<Random<Real[_]>>, x:List<Track>, θ:Global) -> Event {
    auto track <- x.walk();
    while track? {    
      auto o <- track!.y.back();  // observed random variable
      if o.hasDistribution() {
        /* object is detected, compute proposal */
        q:Real[y.size()];
        n:Integer <- 1;
        auto detection <- y.walk();
        while detection? {
          q[n] <- o.pdf(detection!);
          n <- n + 1;
        }
        Q:Real <- sum(q);
          
        /* propose an association */
        if Q > 0.0 {
          q <- q/Q;
          n <~ Categorical(q);  // choose an observation
          auto w <- o.distribution().observe(y.get(n));  // likelihood
          o.distribution().update(y.get(n));
          w <- w - log(y.size());  // prior correction
          w <- w - log(q[n]);  // proposal correction
          y.erase(n);  // remove the observation for future associations
          yield FactorEvent(w);
        } else {
          yield FactorEvent(-inf);  // detected, but all likelihoods (numerically) zero
        }
      }
    }
    
    /* clutter */
    y.size() - 1 ~> Poisson(θ.μ);
    auto clutter <- y.walk();
    while clutter? {
      clutter! ~> Uniform(θ.l, θ.u);
    }
  }
    
  function write(buffer:Buffer) {
    buffer.set("θ", θ);
    buffer.set("z", z);
    buffer.set("y", y);
  }
}
