/*
 * ed Categorical random variate.
 */
final class Categorical(future:Integer?, futureUpdate:Boolean, ρ:Expression<Real[_]>) <
    Distribution<Integer>(future, futureUpdate) {
  /**
   * Category probabilities.
   */
  ρ:Expression<Real[_]> <- ρ;

  function simulate() -> Integer {
    return simulate_categorical(ρ);
  }
  
  function logpdf(x:Integer) -> Real {
    return logpdf_categorical(x, ρ);
  }

  function cdf(x:Integer) -> Real? {
    return cdf_categorical(x, ρ);
  }

  function quantile(p:Real) -> Integer? {
    return quantile_categorical(p, ρ);
  }

  function lower() -> Integer? {
    return 1;
  }

  function upper() -> Integer? {
    return length(ρ);
  }

  function graft() {
    if delay? {
      delay!.prune();
    } else {
      m1:Dirichlet?;
      m2:Restaurant?;
      if (m1 <- ρ.graftDirichlet())? {
        delay <- DirichletCategorical(future, futureUpdate, m1!);
      } else if (m2 <- ρ.graftRestaurant())? {
        delay <- RestaurantCategorical(future, futureUpdate, m2!);
      } else {
        delay <- Categorical(future, futureUpdate, ρ);
      }
    }
  }

  function write(buffer:Buffer) {
    prune();
    buffer.set("class", "Categorical");
    buffer.set("ρ", ρ);
  }
}

function Categorical(future:Integer?, futureUpdate:Boolean,
    ρ:Real[_]) -> Categorical {
  m:Categorical(future, futureUpdate, ρ);
  return m;
}

/**
 * Create categorical distribution.
 */
function Categorical(ρ:Expression<Real[_]>) -> Categorical {
  m:Categorical(ρ);
  return m;
}

/**
 * Create categorical distribution.
 */
function Categorical(ρ:Real[_]) -> Categorical {
  return Categorical(Boxed(ρ));
}
