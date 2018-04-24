/**
 * Binomial distribution.
 */
class Binomial(n:Expression<Integer>, ρ:Expression<Real>) < Random<Integer> {
  /**
   * Number of trials.
   */
  n:Expression<Integer> <- n;

  /**
   * Probability of a true result.
   */
  ρ:Expression<Real> <- ρ;

  function graft() {
    if (ρ.isBeta()) {
      m:DelayBetaBinomial(this, n.value(), ρ.getBeta());
      m.graft();
      delay <- m;
    } else {
      m:DelayBinomial(this, n.value(), ρ.value());
      m.graft();
      delay <- m;
    }
  }
}

/**
 * Create binomial distribution.
 */
function Binomial(n:Expression<Integer>, ρ:Expression<Real>) -> Binomial {
  m:Binomial(n, ρ);
  return m;
}

/**
 * Create binomial distribution.
 */
function Binomial(n:Expression<Integer>, ρ:Real) -> Binomial {
  return Binomial(n, Literal(ρ));
}

/**
 * Create binomial distribution.
 */
function Binomial(n:Integer, ρ:Expression<Real>) -> Binomial {
  return Binomial(Literal(n), ρ);
}

/**
 * Create binomial distribution.
 */
function Binomial(n:Integer, ρ:Real) -> Binomial {
  return Binomial(Literal(n), Literal(ρ));
}
