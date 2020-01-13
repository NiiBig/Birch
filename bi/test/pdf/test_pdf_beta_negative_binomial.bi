/*
 * Test beta-negative-binomial pmf.
 */
program test_pdf_beta_negative_binomial(D:Integer <- 10, N:Integer <- 10000) {
  m:TestBetaNegativeBinomial;
  playDelay.handle(m.simulate());
  test_pdf(m.marginal(), N);
}
