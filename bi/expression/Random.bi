
/**
 * Random variate.
 *
 * - Value: Value type.
 */
class Random<Value> < Expression<Value> {  
  /**
   * Value.
   */
  x:Value?;

  /**
   * Associated distribution.
   */
  dist:Distribution<Value>?;

  /**
   * Value assignment.
   */
  operator <- x:Value {
    assert !this.x?;
    this.x <- x;
    if (dist?) {
      dist!.observe(x);
    }
  }

  /**
   * Optional value assignment.
   */
  operator <- x:Value? {
    assert !this.x?;
    this.x <- x;
    if (x? && dist?) {
      dist!.observe(x!);
    }
  }

  /**
   * Attach a distribution to this random variable.
   */
  function assume(dist:Distribution<Value>) {
    assert !x?;
    assert !this.dist?;
    
    dist.associate(this);
    this.dist <- dist;
  }

  /**
   * Get the value of the random variable, forcing realization if necessary.
   */
  function value() -> Value {
    if !x? {
      assert dist?;
      dist!.value();
    }
    return x!;
  }

  /**
   * Are the values of any random variables within this expression missing?
   */
  function isMissing() -> Boolean {
    return !x?;
  }

  function graftGaussian() -> DelayGaussian? {
    if (!x?) {
      assert dist?;
      return dist!.graftGaussian();
    } else {
      return nil;
    }
  }
    
  function graftBeta() -> DelayBeta? {
    if (!x?) {
      assert dist?;
      return dist!.graftBeta();
    } else {
      return nil;
    }
  }
  
  function graftGamma() -> DelayGamma? {
    if (!x?) {
      assert dist?;
      return dist!.graftGamma();
    } else {
      return nil;
    }
  }
  
  function graftInverseGamma() -> DelayInverseGamma? {
    if (!x?) {
      assert dist?;
      return dist!.graftInverseGamma();
    } else {
      return nil;
    }
  } 
  
  function graftNormalInverseGamma(σ2:Expression<Real>) ->
      DelayNormalInverseGamma? {
    if (!x?) {
      assert dist?;
      return dist!.graftNormalInverseGamma(σ2);
    } else {
      return nil;
    }
  }
  
  function graftDirichlet() -> DelayDirichlet? {
    if (!x?) {
      assert dist?;
      return dist!.graftDirichlet();
    } else {
      return nil;
    }
  }

  function graftRestaurant() -> DelayRestaurant? {
    if (!x?) {
      assert dist?;
      return dist!.graftRestaurant();
    } else {
      return nil;
    }
  }

  function graftMultivariateGaussian() -> DelayMultivariateGaussian? {
    if (!x?) {
      assert dist?;
      return dist!.graftMultivariateGaussian();
    } else {
      return nil;
    }
  }

  function graftMultivariateNormalInverseGamma(σ2:Expression<Real>) ->
      DelayMultivariateNormalInverseGamma? {
    if (!x?) {
      assert dist?;
      return dist!.graftMultivariateNormalInverseGamma(σ2);
    } else {
      return nil;
    }
  }

  function graftDiscrete() -> DelayValue<Integer>? {
    if (!x?) {
      assert dist?;
      return dist!.graftDiscrete();
    } else {
      return nil;
    }
  }
}

/**
 * Create random variate with attached distribution.
 */
function Random(dist:Distribution<Boolean>) -> Random<Boolean> {
  m:Random<Boolean>;
  m.assume(dist);
  return m;
}

/**
 * Create random variate with attached distribution.
 */
function Random(dist:Distribution<Integer>) -> Random<Integer> {
  m:Random<Integer>;
  m.assume(dist);
  return m;
}

/**
 * Create random variate with attached distribution.
 */
function Random(dist:Distribution<Real>) -> Random<Real> {
  m:Random<Real>;
  m.assume(dist);
  return m;
}

/**
 * Create random variate with attached distribution.
 */
function Random(dist:Distribution<Boolean[_]>) -> Random<Boolean[_]> {
  m:Random<Boolean[_]>;
  m.assume(dist);
  return m;
}

/**
 * Create random variate with attached distribution.
 */
function Random(dist:Distribution<Integer[_]>) -> Random<Integer[_]> {
  m:Random<Integer[_]>;
  m.assume(dist);
  return m;
}

/**
 * Create random variate with attached distribution.
 */
function Random(dist:Distribution<Real[_]>) -> Random<Real[_]> {
  m:Random<Real[_]>;
  m.assume(dist);
  return m;
}
