/*
 * Test linear Gaussian-Gaussian cdf evaluations.
 */
program test_cdf_linear_gaussian_gaussian(N:Integer <- 10000) {
  m:TestLinearGaussianGaussian;
  m.play();
  test_cdf(m.marginal(), N);
}
