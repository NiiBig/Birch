/**
 * Lazy subtract.
 */
final class DiscreteSubtract<Left,Right,Value>(left:Expression<Left>,
    right:Expression<Right>) < BinaryExpression<Left,Right,Value>(left, right) {  
  function graft(child:Delay) -> Expression<Value> {
    return left.graft(child) - right.graft(child);
  }

  function doValue(l:Left, r:Right) -> Value {
    return l - r;
  }
  
  function doGradient(d:Value, l:Left, r:Right) -> (Left, Right) {
    return (d, -d);
  }

  function graftDiscrete() -> DelayDiscrete? {
    y:DelayDiscrete? <- graftBoundedDiscrete();
    if (!y?) {
      x:DelayDiscrete?;
      if (x <- left.graftDiscrete())? {
        y <- DelayLinearDiscrete(nil, true, 1, x!, -right.value());
      } else if (x <- right.graftDiscrete())? {
        y <- DelayLinearDiscrete(nil, true, -1, x!, left.value());
      }
    }
    return y;
  }

  function graftBoundedDiscrete() -> DelayBoundedDiscrete? {
    y:DelayBoundedDiscrete?;
    x1:DelayBoundedDiscrete? <- left.graftBoundedDiscrete();
    x2:DelayBoundedDiscrete? <- right.graftBoundedDiscrete();
       
    if x1? && x2? && !(x1!.hasValue()) {    
      // ^ third condition above ensures that x1 is still valid after x2 is
      //   constructed, which will not be the case if left and right share a
      //   common ancestor on the delayed sampling graph
      y <- DelaySubtractBoundedDiscrete(nil, true, x1!, x2!);
    } else if x1? && !(x1!.hasValue()) {
      y <- DelayLinearBoundedDiscrete(nil, true, 1, x1!, -right.value());
    } else if x2? && !(x2!.hasValue()) {
      y <- DelayLinearBoundedDiscrete(nil, true, -1, x2!, left.value());
    }
    return y;
  }
}

/**
 * Lazy subtract.
 */
operator (left:Expression<Integer> - right:Expression<Integer>) ->
    DiscreteSubtract<Integer,Integer,Integer> {
  m:DiscreteSubtract<Integer,Integer,Integer>(left, right);
  return m;
}

/**
 * Lazy subtract.
 */
operator (left:Integer - right:Expression<Integer>) ->
    DiscreteSubtract<Integer,Integer,Integer> {
  return Boxed(left) - right;
}

/**
 * Lazy subtract.
 */
operator (left:Expression<Integer> - right:Integer) ->
    DiscreteSubtract<Integer,Integer,Integer> {
  return left - Boxed(right);
}
