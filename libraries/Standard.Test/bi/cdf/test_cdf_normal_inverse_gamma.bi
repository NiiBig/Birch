/*
 * Test normal-inverse-gamma cdf evaluations.
 */
program test_cdf_normal_inverse_gamma(N:Integer <- 10000) {
  m:TestNormalInverseGamma;
  m.initialize();
  handle(m.simulate());
  test_cdf(m.marginal(), N);
}
