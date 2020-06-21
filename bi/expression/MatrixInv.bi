/**
 * Lazy `inv`.
 */
final class MatrixInv<Single,Value>(x:Single) <
    MatrixUnaryExpression<Single,Value>(x) {
  override function rows() -> Integer {
    return single.rows();
  }
  
  override function columns() -> Integer {
    return single.columns();
  }

  override function doValue() {
    x <- inv(single.value());
  }

  override function doGet() {
    x <- inv(single.get());
  }

  override function doPilot() {
    x <- inv(single.pilot());
  }

  override function doMove(κ:Kernel) {
    x <- inv(single.move(κ));
  }

  override function doGrad() {
    single.grad(-transpose(x!)*d!*transpose(x!));
  }
}

/**
 * Lazy `inv`.
 */
function inv(x:Expression<Real[_,_]>) -> Expression<Real[_,_]> {
  if x.isConstant() {
    return box(matrix(inv(x.value())));
  } else {
    m:MatrixInv<Expression<Real[_,_]>,Real[_,_]>(x);
    return m;
  }
}

/**
 * Lazy `inv`.
 */
function inv(x:Expression<LLT>) -> Expression<LLT> {
  if x.isConstant() {
    return box(llt(inv(x.value())));
  } else {
    m:MatrixInv<Expression<LLT>,LLT>(x);
    return m;
  }
}
