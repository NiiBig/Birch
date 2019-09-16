/*
 * Multivariate linear transformation.
 */
class TransformLinearMultivariate<Value>(A:Value[_,_], c:Value[_]) {
  /**
   * Scale.
   */
  A:Value[_,_] <- A;

  /**
   * Offset.
   */
  c:Value[_] <- c;
  
  function size() -> Integer {
    return length(c);
  }
  
  function leftMultiply(X:Value[_,_]) {
    A <- X*A;
    c <- X*c;
  }

  function add(x:Value[_]) {
    c <- c + x;
  }

  function subtract(x:Value[_]) {
    c <- c - x;
  }
  
  function multiply(x:Value) {
    A <- A*x;
    c <- c*x;
  }

  function divide(x:Value) {
    A <- A/x;
    c <- c/x;
  }
  
  function negateAndAdd(x:Value[_]) {
    A <- -A;
    c <- x - c;
  }
}
