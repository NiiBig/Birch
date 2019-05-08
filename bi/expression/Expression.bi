/**
 * Lazy expression.
 *
 * - Value: Value type.
 */
class Expression<Value> {  
  /**
   * Value conversion.
   */
  operator -> Value {
    return value();
  }

  /**
   * Value evaluation.
   */
  function value() -> Value {
    assert false;
  }
  
  /*
   * Boxed value evaluation.
   */
  function boxed() -> Boxed<Value> {
    return Boxed(value());
  }

  /**
   * Is this expression grafted onto the delayed sampling graph?
   */
  function hasDelay() -> Boolean {
    return false;
  }
  
  /**
   * If this expression is grafted onto the delayed sampling graph, get the
   * node with which it is associated on that graph.
   */
  function getDelay() -> Delay? {
    return nil;
  }
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftGaussian() -> DelayGaussian? {
    return nil;
  }
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftLinearGaussian() -> TransformLinearGaussian? {
    return nil;
  }
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftRidge() -> DelayRidge? {
    return nil;
  }
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftBeta() -> DelayBeta? {
    return nil;
  }
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftGamma() -> DelayGamma? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftScaledGamma() ->  TransformScaledGamma? {
    return nil;
  }
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftInverseGamma() -> DelayInverseGamma? {
    return nil;
  } 
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * - σ2: Variance of the normal distribution for which a
   *       compatible normal-inverse-gamma distribution is sought as prior.
   *
   * Return: The node if successful, nil if not.
   */
  function graftScaledInverseGamma(σ2:Expression<Real>) -> 
      TransformScaledInverseGamma? {
    return nil;
  }
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * - σ2: Variance of the normal distribution for which a
   *       compatible normal-inverse-gamma distribution is sought as prior.
   *
   * Return: The node if successful, nil if not.
   */
  function graftNormalInverseGamma(σ2:Expression<Real>) ->
      DelayNormalInverseGamma? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * - σ2: Variance of the normal distribution for which a
   *       compatible normal-inverse-gamma distribution is sought as prior.
   *
   * Return: The node if successful, nil if not.
   */
  function graftLinearNormalInverseGamma(σ2:Expression<Real>) ->
      TransformLinearNormalInverseGamma? {
    return nil;
  }
  
  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftDirichlet() -> DelayDirichlet? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftRestaurant() -> DelayRestaurant? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftMultivariateGaussian() -> DelayMultivariateGaussian? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftMultivariateLinearGaussian() ->
      TransformMultivariateLinearGaussian? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftMultivariateDotGaussian() ->
      TransformMultivariateDotGaussian? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * - σ2: Variance of the normal distribution for which a
   *       compatible normal-inverse-gamma distribution is sought as prior.
   *
   * Return: The node if successful, nil if not.
   */
  function graftMultivariateScaledInverseGamma(σ2:Expression<Real>) ->
      TransformMultivariateScaledInverseGamma? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * - σ2: Variance of the normal distribution for which a
   *       compatible normal-inverse-gamma distribution is sought as prior.
   *
   * Return: The node if successful, nil if not.
   */
  function graftMultivariateNormalInverseGamma(σ2:Expression<Real>) ->
      DelayMultivariateNormalInverseGamma? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * - σ2: Variance of the normal distribution for which a
   *       compatible normal-inverse-gamma distribution is sought as prior.
   *
   * Return: The node if successful, nil if not.
   */
  function graftMultivariateLinearNormalInverseGamma(σ2:Expression<Real>) ->
      TransformMultivariateLinearNormalInverseGamma? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * - σ2: Variance of the normal distribution for which a
   *       compatible normal-inverse-gamma distribution is sought as prior.
   *
   * Return: The node if successful, nil if not.
   */
  function graftMultivariateDotNormalInverseGamma(σ2:Expression<Real>) ->
      TransformMultivariateDotNormalInverseGamma? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftDiscrete() -> DelayDiscrete? {
    return nil;
  }

  /*
   * Attempt to graft this expression onto the delayed sampling graph.
   *
   * Return: The node if successful, nil if not.
   */
  function graftBoundedDiscrete() -> DelayBoundedDiscrete? {
    return nil;
  }
}
