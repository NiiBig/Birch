import delay.DelayReal;
import math;

/**
 * Beta distribution.
 */
class Beta < DelayReal {
  /**
   * First shape parameter.
   */
  α:Real;

  /**
   * Second shape parameter.
   */
  β:Real;

  function initialize(α:Real, β:Real) {
    super.initialize();
    update(α, β);
  }

  function update(α:Real, β:Real) {
    assert α > 0.0;
    assert β > 0.0;

    this.α <- α;
    this.β <- β;
  }

  function doRealize() {
    if (isMissing()) {
      set(simulate_beta(α, β));
    } else {
      setWeight(observe_beta(x, α, β));
    }
  }

  function tildeLeft() -> Beta {
    simulate();
    return this;
  }
  
  function tildeRight(left:Beta) -> Beta {
    set(left.value());
    observe();
    return this;
  }
}

/**
 * Create a Beta distribution.
 */
function Beta(α:Real, β:Real) -> Beta {
  m:Beta;
  m.initialize(α, β);
  return m;
}
