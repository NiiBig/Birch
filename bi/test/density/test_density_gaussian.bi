/*
 * Test Gaussian density evaluations.
 */
program test_density_gaussian(N:Integer <- 10000) {
  auto μ <- simulate_uniform(-10.0, 10.0);
  auto σ2 <- simulate_uniform(0.0, 10.0);
  auto q <- Gaussian(μ, σ2);  
  test_density(q, N);
}
