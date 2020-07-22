/*
 * Test Student's $t$-distribution cdf evaluations.
 */
program test_cdf_student_t(N:Integer <- 10000) {
  auto k <- simulate_uniform(1.0, 10.0);
  // ^ too heavy-tailed to compute reasonable Riemann sum at k < 1.0
  auto μ <- simulate_uniform(-10.0, 10.0);
  auto σ2 <- simulate_uniform(0.0, 10.0);
    
  auto q <- Student(k, μ, σ2);  
  test_cdf(q, N);
}
