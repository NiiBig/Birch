import delay.DelayReal;
import math;
import random;

/**
 * Uniform distribution.
 */
class Uniform < DelayReal {
  /**
   * Lower bound.
   */
  l:Real;
  
  /**
   * Upper bound.
   */
  u:Real;

  function initialize(l:Real, u:Real) {
    assert l <= u;
  
    super.initialize();
    this.l <- l;
    this.u <- u;
  }

  function update(l:Real, u:Real) {
    assert l <= u;
  
    this.l <- l;
    this.u <- u;
  }

  function doRealize() {
    if (isMissing()) {
      set(random_uniform(l, u));
    } else {
      if (x >= l && x <= u) {
        setWeight(log(1.0/(u - l)));
      } else {
        setWeight(-inf);
      }
    }
  }

  function tilde(left:Uniform) -> Uniform {
    if (left.isNotMissing()) {
      tildeRight(left);
    }
    return this;
  }

  function tildeLeft() -> Uniform {
    simulate();
    return this;
  }
  
  function tildeRight(left:Uniform) -> Uniform {
    set(left.value());
    observe();
    return this;
  }
}

/**
 * Create a Uniform distribution.
 */
function Uniform(l:Real, u:Real) -> Uniform {
  m:Uniform;
  m.initialize(l, u);
  return m;
}
