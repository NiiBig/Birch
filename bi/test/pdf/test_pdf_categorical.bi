/*
 * Test categorical pmf.
 */
program test_pdf_categorical(D:Integer <- 10, N:Integer <- 10000) {
  auto ρ <- simulate_dirichlet(simulate_uniform(1.0, 10.0), D);
  auto π <- Categorical(ρ);
  test_pdf(π, N);
}
