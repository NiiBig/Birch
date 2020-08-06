/*
 * Test matrix linear normal-inverse-Wishart-Gaussian conjugacy.
 */
program test_negative_linear_matrix_normal_inverse_wishart_matrix_gaussian(
    N:Integer <- 10000) {
  m:TestNegativeLinearMatrixNormalInverseWishartMatrixGaussian;
  test_conjugacy(m, N, m.size());
}
