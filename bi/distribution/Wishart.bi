/**
 * Wishart distribution.
 */
final class Wishart(Ψ:Expression<Real[_,_]>, k:Expression<Real>) <
    Distribution<Real[_,_]> {
  /**
   * Scale.
   */
  Ψ:Expression<Real[_,_]> <- Ψ;
  
  /**
   * Degrees of freedom.
   */
  k:Expression<Real> <- k;

  function rows() -> Integer {
    return Ψ.rows();
  }

  function columns() -> Integer {
    return Ψ.columns();
  }

  function simulate() -> Real[_,_] {
    return simulate_wishart(Ψ.value(), k.value());
  }
  
  function logpdf(X:Real[_,_]) -> Real {
    return logpdf_wishart(X, Ψ.value(), k.value());
  }

  function graftWishart() -> Wishart? {
    prune();
    graftFinalize();
    return this;
  }

  function graftFinalize() -> Boolean {
    Ψ.value();
    k.value();
    return true;
  }

  function write(buffer:Buffer) {
    prune();
    buffer.set("class", "Wishart");
    buffer.set("Ψ", Ψ);
    buffer.set("k", k);
  }
}

/**
 * Create Wishart distribution.
 */
function Wishart(Ψ:Expression<Real[_,_]>, k:Expression<Real>) -> Wishart {
  m:Wishart(Ψ, k);
  return m;
}

/**
 * Create Wishart distribution.
 */
function Wishart(Ψ:Expression<Real[_,_]>, k:Real) -> Wishart {
  return Wishart(Ψ, Boxed(k));
}

/**
 * Create Wishart distribution.
 */
function Wishart(Ψ:Real[_,_], k:Expression<Real>) -> Wishart {
  return Wishart(Boxed(Ψ), k);
}

/**
 * Create Wishart distribution.
 */
function Wishart(Ψ:Real[_,_], k:Real) -> Wishart {
  return Wishart(Boxed(Ψ), Boxed(k));
}
