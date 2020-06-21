/**
 * Lazy `join`.
 */
final class MatrixJoin<Value>(x:Expression<Value>[_,_]) <
    MatrixExpression<Value[_,_]> {
  /**
   * Arguments.
   */
  args:Expression<Value>[_,_] <- x;

  override function rows() -> Integer {
    return global.length(args);
  }

  override function doValue() {
    x <- transform(args, \(x:Expression<Value>) -> Value {
        return x.value();
      });
  }

  override function doMakeConstant() {
    for_each(args, \(x:Expression<Value>) { x.makeConstant(); });
  }

  override function doGet() {
    x <- transform(args, \(x:Expression<Value>) -> Value {
        return x.get();
      });
  }
  
  override function doPilot() {
    x <- transform(args, \(x:Expression<Value>) -> Value {
        return x.pilot();
      });
  }

  override function doRestoreCount() {
    for_each(args, \(x:Expression<Value>) { x.restoreCount(); });
  }

  override function doMove(κ:Kernel) {
    x <- transform(args, \(x:Expression<Value>) -> Value {
        return x.move(κ);
      });
  }
  
  override function doGrad() {
    for_each(args, D!, \(x:Expression<Value>, d:Value) { x.grad(d); });
  }

  override function doPrior(vars:RaggedArray<DelayExpression>) ->
      Expression<Real>? {
    p:Expression<Real>?;
    auto R <- rows();
    auto C <- columns();
    for i in 1..R {
      for j in 1..C {
        auto q <- args[i,j].prior(vars);
        if q? {
          if p? {
            p <- p! + q!;
          } else {
            p <- q;
          }
        }
      }
    }
    return p;
  }
}

/**
 * Lazy `join`. Converts a matrix of scalar expressions into a single matrix
 * expression.
 */
function join(X:Expression<Real>[_,_]) -> Expression<Real[_,_]> {
  m:MatrixJoin<Real>(X);
  return m;
}

/**
 * Lazy `split`. Converts a matrix expression into a matrix of scalar
 * expressions.
 */
function split(X:Expression<Real[_,_]>) -> Expression<Real>[_,_] {
  return matrix(\(i:Integer, j:Integer) -> Expression<Real> {
        return X.element(i, j);
      }, X.rows(), X.columns());
}
