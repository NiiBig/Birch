/**
 * Lazy `diagonal`.
 */
final class Diagonal<Left,Right,Value>(x:Left, n:Right) <
    MatrixBinaryExpression<Left,Right,Value>(x, n) {  
  override function rows() -> Integer {
    return right.value();
  }
  
  override function columns() -> Integer {
    return right.value();
  }

  override function doValue() {
    x <- diagonal(left.value(), right.value());
  }

  override function doGet() {
    x <- diagonal(left.get(), right.get());
  }

  override function doPilot() {
    x <- diagonal(left.pilot(), right.pilot());
  }

  override function doMove(κ:Kernel) {
    x <- diagonal(left.move(κ), right.move(κ));
  }

  override function doGrad() {
    left.grad(trace(D!));
  }
}

/**
 * Lazy `diagonal`.
 */
function diagonal(x:Expression<Real>, n:Expression<Integer>) ->
    Expression<Real[_,_]> {
  if x.isConstant() && n.isConstant() {
    return box(diagonal(x.value(), n.value()));
  } else {
    m:Diagonal<Expression<Real>,Expression<Integer>,Real[_,_]>(x, n);
    return m;
  }
}

/**
 * Lazy `diagonal`.
 */
function diagonal(x:Real, n:Expression<Integer>) -> Expression<Real[_,_]> {
  if n.isConstant() {
    return box(diagonal(x, n.value()));
  } else {
    return diagonal(box(x), n);
  }
}

/**
 * Lazy `diagonal`.
 */
function diagonal(x:Expression<Real>, n:Integer) -> Expression<Real[_,_]> {
  if x.isConstant() {
    return box(diagonal(x.value(), n));
  } else {
    return diagonal(x, box(n));
  }
}
