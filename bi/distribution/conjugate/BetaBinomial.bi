/**
 * Binomial with conjugate prior on success probability. When the success
 * probability is known, this is simply a Binomial distribution. When unknown,
 * 
 */
class BetaBinomial < DelayInteger {
  /**
   * Number of trials.
   */
  n:Integer;

  /**
   * Probability of a true result.
   */
  ρ:Beta;

  function initialize(n:Integer, ρ:Beta) {
    super.initialize(ρ);
    this.n <- n;
    this.ρ <- ρ;
  }
  
  function doMarginalize() {
    //
  }
  
  function doCondition() {
    ρ.update(ρ.α + x, ρ.β + n - x);
  }

  function doRealize() {
    if (ρ.isRealized()) {
      if (isMissing()) {
        set(simulate_binomial(n, ρ));
      } else {
        setWeight(observe_binomial(x, n, ρ));
      }
    } else {
      if (isMissing()) {
        set(simulate_beta_binomial(n, ρ.α, ρ.β));
      } else {
        setWeight(observe_beta_binomial(x, n, ρ.α, ρ.β));
      }
    }
  }

  function tildeLeft() -> BetaBinomial {
    simulate();
    return this;
  }
  
  function tildeRight(left:BetaBinomial) -> BetaBinomial {
    set(left.value());
    observe();
    return this;
  }
}

/**
 * Create beta-binomial distribution.
 */
function Binomial(n:Integer, ρ:Beta) -> BetaBinomial {
  x:BetaBinomial;
  x.initialize(n, ρ);
  return x;
}
