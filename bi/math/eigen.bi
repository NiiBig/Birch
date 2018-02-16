/*
 * Birch uses the Eigen library <https://eigen.tuxfamily.org> for linear
 * algebra support. Eigen is tightly integrated with support from libbirch,
 * in order to preserve the lazy evaluation of Eigen that is a critical
 * feature of its performance.
 */

operator (x:Real[_] == y:Real[_]) -> Boolean;
operator (x:Real[_] != y:Real[_]) -> Boolean;
operator (X:Real[_,_] == Y:Real[_,_]) -> Boolean;
operator (X:Real[_,_] != Y:Real[_,_]) -> Boolean;

operator (-x:Real[_]) -> Real[_];
operator (+x:Real[_]) -> Real[_];
operator (-X:Real[_,_]) -> Real[_,_];
operator (+X:Real[_,_]) -> Real[_,_];

operator (x:Real[_] + y:Real[_]) -> Real[_];
operator (x:Real[_] - y:Real[_]) -> Real[_];
operator (X:Real[_,_] + Y:Real[_,_]) -> Real[_,_];
operator (X:Real[_,_] - Y:Real[_,_]) -> Real[_,_];

operator (x:Real[_]*y:Real[_]) -> Real[_];
operator (X:Real[_,_]*y:Real[_]) -> Real[_];
operator (x:Real[_]*Y:Real[_,_]) -> Real[_,_];
operator (X:Real[_,_]*Y:Real[_,_]) -> Real[_,_];

operator (x:Real*y:Real[_]) -> Real[_];
operator (x:Real[_]*y:Real) -> Real[_];
operator (x:Real*Y:Real[_,_]) -> Real[_,_];
operator (X:Real[_,_]*y:Real) -> Real[_,_];

operator (x:Real[_]/y:Real) -> Real[_];
operator (X:Real[_,_]/y:Real) -> Real[_,_];

operator (x:Integer[_] == y:Integer[_]) -> Boolean;
operator (x:Integer[_] != y:Integer[_]) -> Boolean;
operator (X:Integer[_,_] == Y:Integer[_,_]) -> Boolean;
operator (X:Integer[_,_] != Y:Integer[_,_]) -> Boolean;

operator (-x:Integer[_]) -> Integer[_];
operator (+x:Integer[_]) -> Integer[_];
operator (-X:Integer[_,_]) -> Integer[_,_];
operator (+X:Integer[_,_]) -> Integer[_,_];

operator (x:Integer[_] + y:Integer[_]) -> Integer[_];
operator (x:Integer[_] - y:Integer[_]) -> Integer[_];
operator (X:Integer[_,_] + Y:Integer[_,_]) -> Integer[_,_];
operator (X:Integer[_,_] - Y:Integer[_,_]) -> Integer[_,_];

operator (x:Integer[_]*y:Integer[_]) -> Integer[_];
operator (X:Integer[_,_]*y:Integer[_]) -> Integer[_];
operator (x:Integer[_]*Y:Integer[_,_]) -> Integer[_,_];
operator (X:Integer[_,_]*Y:Integer[_,_]) -> Integer[_,_];

operator (x:Integer*y:Integer[_]) -> Integer[_];
operator (x:Integer[_]*y:Integer) -> Integer[_];
operator (x:Integer*Y:Integer[_,_]) -> Integer[_,_];
operator (X:Integer[_,_]*y:Integer) -> Integer[_,_];

operator (x:Boolean[_] == y:Boolean[_]) -> Boolean;
operator (x:Boolean[_] != y:Boolean[_]) -> Boolean;
operator (X:Boolean[_,_] == Y:Boolean[_,_]) -> Boolean;
operator (X:Boolean[_,_] != Y:Boolean[_,_]) -> Boolean;

operator (-x:Boolean[_]) -> Boolean[_];
operator (+x:Boolean[_]) -> Boolean[_];
operator (-X:Boolean[_,_]) -> Boolean[_,_];
operator (+X:Boolean[_,_]) -> Boolean[_,_];

operator (x:Boolean[_] + y:Boolean[_]) -> Boolean[_];
operator (x:Boolean[_] - y:Boolean[_]) -> Boolean[_];
operator (X:Boolean[_,_] + Y:Boolean[_,_]) -> Boolean[_,_];
operator (X:Boolean[_,_] - Y:Boolean[_,_]) -> Boolean[_,_];

operator (x:Boolean[_]*y:Boolean[_]) -> Boolean[_];
operator (X:Boolean[_,_]*y:Boolean[_]) -> Boolean[_];
operator (x:Boolean[_]*Y:Boolean[_,_]) -> Boolean[_,_];
operator (X:Boolean[_,_]*Y:Boolean[_,_]) -> Boolean[_,_];

operator (x:Boolean*y:Boolean[_]) -> Boolean[_];
operator (x:Boolean[_]*y:Boolean) -> Boolean[_];
operator (x:Boolean*Y:Boolean[_,_]) -> Boolean[_,_];
operator (X:Boolean[_,_]*y:Boolean) -> Boolean[_,_];

/**
 * Dot product of a vector with itself.
 */
function dot(x:Real[_]) -> Real;
function dot(x:Integer[_]) -> Integer;
function dot(x:Boolean[_]) -> Boolean;

/**
 * Transpose of a matrix.
 */
function transpose(X:Real[_,_]) -> Real[_,_];
function transpose(X:Integer[_,_]) -> Integer[_,_];
function transpose(X:Boolean[_,_]) -> Boolean[_,_];

/**
 * Norm of a vector.
 */
function norm(x:Real[_]) -> Real;

/**
 * Determinant of a matrix.
 */
function determinant(X:Real[_,_]) -> Real;

/**
 * Inverse of a matrix.
 */
function inverse(X:Real[_,_]) -> Real[_,_];

/*
 * for the below functions, need to assign the result to a new matrix, as, it
 * seems, they return a view of a matrix that will be destroyed on return
 */
/**
 * `LL^T` Cholesky decomposition of a matrix.
 */
function llt(X:Real[_,_]) -> Real[_,_] {
  assert rows(X) == columns(X);
  
  L:Real[rows(X),columns(X)];
  cpp{{
  L_.toEigen() = X_.toEigen().llt().matrixL();
  }}
  return L;
}

/**
 * Solve a system of equations.
 */
function solve(X:Real[_,_], y:Real[_]) -> Real[_] {
  assert columns(X) == length(y);
  
  z:Real[rows(X)];
  cpp{{
  z_.toEigen() = X_.toEigen().colPivHouseholderQr().solve(y_.toEigen());
  }}
  return z;
}

/**
 * Solve a system of equations.
 */
function solve(X:Real[_,_], Y:Real[_,_]) -> Real[_,_] {
  assert columns(X) == rows(Y);
  
  Z:Real[rows(Y),columns(Y)];
  cpp{{
  Z_.toEigen() = X_.toEigen().colPivHouseholderQr().solve(Y_.toEigen());
  }}
  return Z;
}
