/*
 * Test scaled gamma-Poisson conjugacy.
 */
program test_negative_scaled_gamma_poisson(N:Integer <- 10000) {
  m:TestNegativeScaledGammaPoisson;
  test_conjugacy(m, N, 2);
}
