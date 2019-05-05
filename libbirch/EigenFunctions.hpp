/**
 * @file
 *
 * Wrappers for Eigen functions that preserve its lazy evaluation.
 */
#pragma once

#include "libbirch/Eigen.hpp"
#include "libbirch/Array.hpp"

namespace bi {

template<class Type>
auto norm(const Eigen::MatrixBase<Type>& o) {
  return o.norm();
}

template<class Type, class Frame>
auto norm(const libbirch::Array<Type,Frame>& o) {
  return norm(o.toEigen());
}

template<class Type>
auto dot(const Eigen::MatrixBase<Type>& o) {
  return o.squaredNorm();
}

template<class Type, class Frame>
auto dot(const libbirch::Array<Type,Frame>& o) {
  return dot(o.toEigen());
}

template<class Type1, class Type2>
auto dot(const Eigen::MatrixBase<Type1>& o1,
    const Eigen::MatrixBase<Type2>& o2) {
  return o1.dot(o2);
}

template<class Type1, class Type2, class Frame>
auto dot(const Eigen::MatrixBase<Type1>& o1,
    const libbirch::Array<Type2,Frame>& o2) {
  return dot(o1, o2.toEigen());
}

template<class Type1, class Frame, class Type2>
auto dot(const libbirch::Array<Type1,Frame>& o1,
    const Eigen::MatrixBase<Type2>& o2) {
  return dot(o1.toEigen(), o2);
}

template<class Type1, class Frame1, class Type2, class Frame2>
auto dot(const libbirch::Array<Type1,Frame1>& o1,
    const libbirch::Array<Type2,Frame2>& o2) {
  return dot(o1.toEigen(), o2.toEigen());
}

template<class Type>
auto det(const Eigen::MatrixBase<Type>& o) {
  return o.determinant();
}

template<class Type, class Frame>
auto det(const libbirch::Array<Type,Frame>& o) {
  return det(o.toEigen());
}

template<class Type>
auto trans(const Eigen::MatrixBase<Type>& o) {
  return o.transpose();
}

template<class Type, class Frame>
auto trans(const libbirch::Array<Type,Frame>& o) {
  return trans(o.toEigen());
}

template<class Type>
auto inv(const Eigen::MatrixBase<Type>& o) {
  return o.inverse();
}

template<class Type, class Frame>
auto inv(const libbirch::Array<Type,Frame>& o) {
  return inv(o.toEigen());
}

template<class Type1, class Type2>
auto solve(const Eigen::MatrixBase<Type1>& o1,
    const Eigen::MatrixBase<Type2>& o2) {
  return o1.householderQr().solve(o2).eval();
}

template<class Type1, class Type2, class Frame2>
auto solve(const Eigen::MatrixBase<Type1>& o1,
    const libbirch::Array<Type2,Frame2>& o2) {
  return solve(o1, o2.toEigen());
}

template<class Type1, class Frame1, class Type2>
auto solve(const libbirch::Array<Type1,Frame1>& o1,
    const Eigen::MatrixBase<Type2>& o2) {
  return solve(o1.toEigen(), o2);
}

template<class Type1, class Frame1, class Type2, class Frame2>
auto solve(const libbirch::Array<Type1,Frame1>& o1,
    const libbirch::Array<Type2,Frame2>& o2) {
  return solve(o1.toEigen(), o2.toEigen());
}

}
