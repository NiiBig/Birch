/*
 * Test linear normal-inverse-gamma Gaussian cdf evaluations.
 */
program test_cdf_linear_multivariate_normal_inverse_gamma_gaussian(
    N:Integer <- 10000) {
  m:TestLinearMultivariateNormalInverseGammaGaussian;
  m.initialize();
  handle(m.simulate());
  test_cdf(m.marginal(), N);
}
