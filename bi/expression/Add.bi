/**
 * Lazy add.
 */
final class Add<Left,Right,Value>(left:Expression<Left>,
    right:Expression<Right>) < BinaryExpression<Left,Right,Value>(left, right) {  
  override function computeValue(l:Left, r:Right) -> Value {
    return l + r;
  }
  
  override function computeGrad(d:Value, l:Left, r:Right) -> (Left, Right) {
    return (d, d);
  }

  override function graftLinearGaussian() -> TransformLinear<Gaussian>? {
    y:TransformLinear<Gaussian>?;
    z:Gaussian?;
    
    if (y <- left.graftLinearGaussian())? {
      y!.add(right);
    } else if (y <- right.graftLinearGaussian())? {
      y!.add(left);
    } else if (z <- left.graftGaussian())? {
      y <- TransformLinear<Gaussian>(box(1.0), z!, right);
    } else if (z <- right.graftGaussian())? {
      y <- TransformLinear<Gaussian>(box(1.0), z!, left);
    }
    return y;
  }

  override function graftDotGaussian() -> TransformDot<MultivariateGaussian>? {
    y:TransformDot<MultivariateGaussian>?;
    if (y <- left.graftDotGaussian())? {
      y!.add(right);
    } else if (y <- right.graftDotGaussian())? {
      y!.add(left);
    }
    return y;
  }
  
  override function graftLinearNormalInverseGamma(compare:Distribution<Real>) ->
      TransformLinear<NormalInverseGamma>? {
    y:TransformLinear<NormalInverseGamma>?;
    z:NormalInverseGamma?;

    if (y <- left.graftLinearNormalInverseGamma(compare))? {
      y!.add(right);
    } else if (y <- right.graftLinearNormalInverseGamma(compare))? {
      y!.add(left);
    } else if (z <- left.graftNormalInverseGamma(compare))? {
      y <- TransformLinear<NormalInverseGamma>(box(1.0), z!, right);
    } else if (z <- right.graftNormalInverseGamma(compare))? {
      y <- TransformLinear<NormalInverseGamma>(box(1.0), z!, left);
    }
    return y;
  }

  override function graftDotNormalInverseGamma(compare:Distribution<Real>) ->
      TransformDot<MultivariateNormalInverseGamma>? {
    y:TransformDot<MultivariateNormalInverseGamma>?;
    
    if (y <- left.graftDotNormalInverseGamma(compare))? {
      y!.add(right);
    } else if (y <- right.graftDotNormalInverseGamma(compare))? {
      y!.add(left);
    }
    return y;
  }
}

/**
 * Lazy add.
 */
operator (left:Expression<Real> + right:Expression<Real>) ->
    Expression<Real> {
  if left.isConstant() && right.isConstant() {
    return box(left.value() + right.value());
  } else {
    m:Add<Real,Real,Real>(left, right);
    return m;
  }
}

/**
 * Lazy add.
 */
operator (left:Real + right:Expression<Real>) -> Expression<Real> {
  if right.isConstant() {
    return box(left + right.value());
  } else {
    return box(left) + right;
  }
}

/**
 * Lazy add.
 */
operator (left:Expression<Real> + right:Real) -> Expression<Real> {
  if left.isConstant() {
    return box(left.value() + right);
  } else {
    return left + box(right);
  }
}
