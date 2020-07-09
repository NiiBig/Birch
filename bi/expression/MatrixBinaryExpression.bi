/**
 * Matrix binary expression.
 *
 * - Left: Left argument type.
 * - Right: Right argument type.
 * - Value: Value type.
 */
abstract class MatrixBinaryExpression<Left,Right,Value>(left:Left,
    right:Right) < MatrixExpression<Value> {  
  /**
   * Left argument.
   */
  left:Left? <- left;
  
  /**
   * Right argument.
   */
  right:Right? <- right;

  final override function depth() -> Integer {
    auto leftDepth <- 0;
    auto rightDepth <- 0;
    if left? {
    assert !isConstant();
      leftDepth <- left!.depth();
    }
    if right? {
      rightDepth <- right!.depth();
    }
    return max(leftDepth, rightDepth) + 1;
  }

  final override function doDetach() {
    left <- nil;
    right <- nil;
  }

  final override function doMakeConstant() {
    left!.makeConstant();
    right!.makeConstant();
  }
  
  final override function doPrior(vars:RaggedArray<DelayExpression>) ->
      Expression<Real>? {
    auto l <- left!.prior(vars);
    auto r <- right!.prior(vars);
    if l? && r? {
      return l! + r!;
    } else if l? {
      return l!;
    } else if r? {
      return r!;
    } else {
      return nil;
    }
  }
}
