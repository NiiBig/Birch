/**
 * Transformation.
 *
 * - x: Vector.
 * - f: Operator.
 */
function transform(x:Real[_], f:@(Real) -> Real) -> Real[_] {
  // in C++17 can use std::transform
  y:Real[length(x)];
  for i:Integer in 1..length(x) {
    y[i] <- f(x[i]);
  }
  return y;
}

/**
 * Transformation.
 *
 * - x: Vector.
 * - f: Operator.
 */
function transform(x:Integer[_], f:@(Integer) -> Integer) -> Integer[_] {
  // in C++17 can use std::transform
  y:Integer[length(x)];
  for i:Integer in 1..length(x) {
    y[i] <- f(x[i]);
  }
  return y;
}

/**
 * Transformation.
 *
 * - x: Vector.
 * - f: Operator.
 */
function transform(x:Boolean[_], f:@(Boolean) -> Boolean) -> Boolean[_] {
  // in C++17 can use std::transform
  y:Boolean[length(x)];
  for i:Integer in 1..length(x) {
    y[i] <- f(x[i]);
  }
  return y;
}

/**
 * Transformation.
 *
 * - X: Matrix.
 * - f: Operator.
 */
function transform(X:Real[_,_], f:@(Real) -> Real) -> Real[_,_] {
  // in C++17 can use std::transform
  Y:Real[_,_];
  for i:Integer in 1..rows(X) {
    for j:Integer in 1..columns(X) {
      Y[i,j] <- f(X[i,j]);
    }
  }
  return Y;
}

/**
 * Transformation.
 *
 * - X: Matrix.
 * - f: Operator.
 */
function transform(X:Integer[_,_], f:@(Integer) -> Integer) -> Integer[_,_] {
  // in C++17 can use std::transform
  Y:Integer[_,_];
  for i:Integer in 1..rows(X) {
    for j:Integer in 1..columns(X) {
      Y[i,j] <- f(X[i,j]);
    }
  }
  return Y;
}

/**
 * Transformation.
 *
 * - X: Matrix.
 * - f: Operator.
 */
function transform(X:Boolean[_,_], f:@(Boolean) -> Boolean) -> Boolean[_,_] {
  // in C++17 can use std::transform
  Y:Boolean[_,_];
  for i:Integer in 1..rows(X) {
    for j:Integer in 1..columns(X) {
      Y[i,j] <- f(X[i,j]);
    }
  }
  return Y;
}

/**
 * Reduction.
 *
 * - x: Vector.
 * - init: Initial value.
 * - op: Operator.
 */
function reduce(x:Real[_], init:Real, op:@(Real, Real) -> Real) -> Real {
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  // return std::reduce(first, last, init_, op_);
  // ^ C++17
  return std::accumulate(first, last, init_, op_);
  }}
}

/**
 * Reduction.
 *
 * - x: Vector.
 * - init: Initial value.
 * - op: Operator.
 */
function reduce(x:Integer[_], init:Integer,
    op:@(Integer, Integer) -> Integer) -> Integer {
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  // return std::reduce(first, last, init_, op_);
  // ^ C++17
  return std::accumulate(first, last, init_, op_);
  }}
}

/**
 * Reduction.
 *
 * - x: Vector.
 * - init: Initial value.
 * - op: Operator.
 */
function reduce(x:Boolean[_], init:Boolean,
    op:@(Boolean, Boolean) -> Boolean) -> Boolean {
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  // return std::reduce(first, last, init_, op_);
  // ^ C++17
  return std::accumulate(first, last, init_, op_);
  }}
}

/**
 * Inclusive scan.
 *
 * - x: Vector.
 * - op: Operator.
 */
function inclusive_scan(x:Real[_], op:@(Real, Real) -> Real) -> Real[_] {
  y:Real[length(x)];
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  // std::inclusive_scan(first, last, y_.begin(), op_);
  // ^ C++17
  std::partial_sum(first, last, y_.begin(), op_);
  }}
  return y;
}

/**
 * Inclusive scan.
 *
 * - x: Vector.
 * - op: Operator.
 */
function inclusive_scan(x:Integer[_], op:@(Integer, Integer) -> Integer) ->
    Integer[_] {
  y:Integer[length(x)];
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  // std::inclusive_scan(first, last, y_.begin(), op_);
  // ^ C++17
  std::partial_sum(first, last, y_.begin(), op_);
  }}
  return y;
}

/**
 * Inclusive scan.
 *
 * - x: Vector.
 * - op: Operator.
 */
function inclusive_scan(x:Boolean[_], op:@(Boolean, Boolean) -> Boolean) ->
    Boolean[_] {
  y:Boolean[length(x)];
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  // std::inclusive_scan(first, last, y_.begin(), op_);
  // ^ C++17
  std::partial_sum(first, last, y_.begin(), op_);
  }}
  return y;
}

/**
 * Exclusive scan.
 *
 * - x: Vector.
 * - init: Initial value.
 * - op: Operator.
 */
function exclusive_scan(x:Real[_], init:Real,
    op:@(Real, Real) -> Real) -> Real[_] {
  assert length(x) > 0;
  y:Real[length(x)];
  //cpp{{
  // auto first = x_.begin();
  // auto last = first + x_.size();
  // std::exclusive_scan(first, last, y_.begin(), init_, op_);
  // ^ C++17
  //}}
  y[1] <- init;
  for (n:Integer in 2..length(x)) {
    y[n] <- y[n - 1] + x[n - 1];
  }
  return y;
}

/**
 * Exclusive scan.
 *
 * - x: Vector.
 * - init: Initial value.
 * - op: Operator.
 */
function exclusive_scan(x:Integer[_], init:Integer,
    op:@(Integer, Integer) -> Integer) -> Integer[_] {
  assert length(x) > 0;
  y:Integer[length(x)];
  //cpp{{
  // auto first = x_.begin();
  // auto last = first + x_.size();
  // std::exclusive_scan(first, last, y_.begin(), init_, op_);
  // ^ C++17
  //}}
  y[1] <- init;
  for (n:Integer in 2..length(x)) {
    y[n] <- y[n - 1] + x[n - 1];
  }
  return y;
}

/**
 * Exclusive scan.
 *
 * - x: Vector.
 * - init: Initial value.
 * - op: Operator.
 */
function exclusive_scan(x:Boolean[_], init:Boolean,
    op:@(Boolean, Boolean) -> Boolean) -> Boolean[_] {
  assert length(x) > 0;
  y:Boolean[length(x)];
  //cpp{{
  // auto first = x_.begin();
  // auto last = first + x_.size();
  // std::exclusive_scan(first, last, y_.begin(), init_, op_);
  // ^ C++17
  //}}
  y[1] <- init;
  for (n:Integer in 2..length(x)) {
    y[n] <- y[n - 1] + x[n - 1];
  }
  return y;
}

/**
 * Adjacent difference.
 *
 * - x: Vector.
 * - op: Operator.
 */
function adjacent_difference(x:Real[_],
    op:@(Real, Real) -> Real) -> Real[_] {
  y:Real[length(x)];
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  std::adjacent_difference(first, last, y_.begin(), op_);
  }}
  return y;
}

/**
 * Adjacent difference.
 *
 * - x: Vector.
 * - op: Operator.
 */
function adjacent_difference(x:Integer[_],
    op:@(Integer, Integer) -> Integer) -> Integer[_] {
  y:Integer[length(x)];
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  std::adjacent_difference(first, last, y_.begin(), op_);
  }}
  return y;
}

/**
 * Adjacent difference.
 *
 * - x: Vector.
 * - op: Operator.
 */
function adjacent_difference(x:Boolean[_],
    op:@(Boolean, Boolean) -> Boolean) -> Boolean[_] {
  y:Boolean[length(x)];
  cpp{{
  auto first = x_.begin();
  auto last = first + x_.size();
  std::adjacent_difference(first, last, y_.begin(), op_);
  }}
  return y;
}

/**
 * Sort.
 *
 * - x: Vector.
 */
function sort(x:Real[_]) -> Real[_] {
  y:Real[_] <- x;
  cpp{{
  auto first = y_.begin();
  auto last = first + y_.size();
  std::sort(first, last);
  }}
  return y;
}

/**
 * Sort.
 *
 * - x: Vector.
 */
function sort(x:Integer[_]) -> Integer[_] {
  y:Integer[_] <- x;
  cpp{{
  auto first = y_.begin();
  auto last = first + y_.size();
  std::sort(first, last);
  }}
  return y;
}

/**
 * Sort.
 *
 * - x: Vector.
 */
function sort(x:Boolean[_]) -> Boolean[_] {
  y:Boolean[_] <- x;
  cpp{{
  auto first = y_.begin();
  auto last = first + y_.size();
  std::sort(first, last);
  }}
  return y;
}
