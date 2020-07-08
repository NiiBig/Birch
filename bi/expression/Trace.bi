/**
 * Lazy `trace`.
 */
final class Trace<Single,Value>(x:Single) <
    ScalarUnaryExpression<Single,Value>(x) {
  override function doValue() {
    x <- trace(single!.value());
  }

  override function doPilot() {
    x <- trace(single!.pilot());
  }

  override function doMove(κ:Kernel) {
    x <- trace(single!.move(κ));
  }

  override function doGrad() {
    single!.grad(diagonal(d!, single!.rows()));
  }
}

/**
 * Lazy `trace`.
 */
function trace(x:Expression<LLT>) -> Expression<Real> {
  if x.isConstant() {
    return box(trace(x.value()));
  } else {
    return construct<Trace<Expression<LLT>,Real>>(x);
  }
}

/**
 * Lazy `trace`.
 */
function trace(x:Expression<Real[_,_]>) -> Expression<Real> {
  if x.isConstant() {
    return box(trace(x.value()));
  } else {
    return construct<Trace<Expression<Real[_,_]>,Real>>(x);
  }
}
