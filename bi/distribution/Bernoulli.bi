import delay.DelayBoolean;
import math;

/**
 * Bernoulli distribution.
 */
class Bernoulli < DelayBoolean {
  /**
   * Probability of a true result.
   */
  ρ:Real;

  function initialize(ρ:Real) {
    assert 0.0 <= ρ && ρ <= 1.0;
  
    super.initialize();
    this.ρ <- ρ;
  }

  function update(ρ:Real) {
    assert 0.0 <= ρ && ρ <= 1.0;
  
    this.ρ <- ρ;
  }

  function doRealize() {
    if (isMissing()) {
      set(simulate_bernoulli(ρ));
    } else {
      if (x) {
        setWeight(log(ρ));
       } else {
        setWeight(log(1.0 - ρ));
      }
    }
  }

  function tildeLeft() -> Bernoulli {
    simulate();
    return this;
  }
  
  function tildeRight(left:Bernoulli) -> Bernoulli {
    set(left.value());
    observe();
    return this;
  }
}

/**
 * Create.
 */
function Bernoulli(ρ:Real) -> Bernoulli {
  m:Bernoulli;
  m.initialize(ρ);
  return m;
}
