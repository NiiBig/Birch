/**
 * Lazy add.
 */
final class DiscreteAdd<Left,Right,Value>(left:Expression<Left>,
    right:Expression<Right>) < BinaryExpression<Left,Right,Value>(left, right) {  
  function doValue(l:Left, r:Right) -> Value {
    return l + r;
  }
  
  function doGradient(d:Value, l:Left, r:Right) -> (Left, Right) {
    return (d, d);
  }

  function graftDiscrete() -> Discrete? {
    y:Discrete? <- graftBoundedDiscrete();
    if !y? {
      x:Discrete?;
      if (x <- left.graftDiscrete())? {
        y <- LinearDiscrete(1, x!, right.value());
      } else if (x <- right.graftDiscrete())? {
        y <- LinearDiscrete(1, x!, left.value());
      }
    }
    return y;
  }

  function graftBoundedDiscrete() -> BoundedDiscrete? {
    y:BoundedDiscrete?;
    x1:BoundedDiscrete? <- left.graftBoundedDiscrete();
    x2:BoundedDiscrete? <- right.graftBoundedDiscrete();

    if x1? && x2? && !(x1!.hasValue()) {
      // ^ third condition above ensures that x1 is still valid after x2 is
      //   constructed, which will not be the case if left and right share a
      //   common ancestor on the delayed sampling graph
      y <- AddBoundedDiscrete(x1!, x2!);
    } else if x1? && !(x1!.hasValue()) {
      y <- LinearBoundedDiscrete(1, x1!, right.value());
    } else if x2? && !(x2!.hasValue()) {
      y <- LinearBoundedDiscrete(1, x2!, left.value());
    }
    return y;
  }
}

/**
 * Lazy add.
 */
operator (left:Expression<Integer> + right:Expression<Integer>) ->
    DiscreteAdd<Integer,Integer,Integer> {
  m:DiscreteAdd<Integer,Integer,Integer>(left, right);
  return m;
}

/**
 * Lazy add.
 */
operator (left:Integer + right:Expression<Integer>) ->
    DiscreteAdd<Integer,Integer,Integer> {
  return Boxed(left) + right;
}

/**
 * Lazy add.
 */
operator (left:Expression<Integer> + right:Integer) ->
    DiscreteAdd<Integer,Integer,Integer> {
  return left + Boxed(right);
}
