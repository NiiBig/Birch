/**
 * Multivariate unary expression.
 *
 * - Single: Single argument type.
 * - Value: Value type.
 */
abstract class MultivariateUnaryExpression<Single,Value>(single:Single) <
    MultivariateExpression<Value> {  
  /**
   * Single argument.
   */
  single:Single <- single;

  final override function doMakeConstant() {
    single.makeConstant();
  }
 
  final override function doRestoreCount() {
    single.restoreCount();
  }

  final override function doPrior(vars:RaggedArray<DelayExpression>) ->
      Expression<Real>? {
    return single.prior(vars);
  }
}
