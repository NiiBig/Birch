/**
 * Uniform distribution.
 */
class Uniform < Random<Real> {
  /**
   * Lower bound.
   */
  l:Real;
  
  /**
   * Upper bound.
   */
  u:Real;

  function initialize(l:Real, u:Real) {
    super.initialize();
    update(l, u);
  }

  function update(l:Real, u:Real) {
    assert l <= u;
  
    this.l <- l;
    this.u <- u;
  }

  function doRealize() {
    if (isMissing()) {
      set(simulate_uniform(l, u));
    } else {
      setWeight(observe_uniform(x, l, u));
    }
  }

  function tildeLeft() -> Uniform {
    simulate();
    return this;
  }
}

/**
 * Create a uniform distribution.
 */
function Uniform(l:Real, u:Real) -> Uniform {
  m:Uniform;
  m.initialize(l, u);
  return m;
}
