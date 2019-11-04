/*
 * Test a continuous distribution pdf.
 *
 * - π: The target distribution. 
 * - D: Number of dimensions.
 * - N: Number of samples.
 * - B: Metropolis sampler burn-in.
 * - S: Metropolis sampler skip.
 */
function test_pdf(π:Distribution<Real[_]>, D:Integer, N:Integer, B:Integer,
    S:Integer) {
  auto q <- Gaussian(vector(0.0, D), diagonal(0.01, D));
  
  X1:Real[N,D];
  X2:Real[N,D];
  
  /* iid samples */
  for auto n in 1..N {
    X1[n,1..D] <- π.simulate();
  }
  
  /* Metropolis sampler, initialize */
  auto x <- π.simulate();
  
  /* burn in */
  auto μ <- vector(0.0, D);
  auto Σ <- matrix(0.0, D, D);
  for auto b in 1..B {
    auto x' <- x + q.simulate();
    if simulate_uniform(0.0, 1.0) <= π.pdf(x')/π.pdf(x) {
      x <- x';  // accept
    }
    μ <- μ + x;
    Σ <- Σ + x*transpose(x);
  }
  
  /* adapt */
  μ <- μ/N;
  Σ <- Σ/N - μ*transpose(μ);
  q <- Gaussian(vector(0.0, D), (pow(2.4, 2.0)/D)*Σ);

  /* sample */
  auto A <- 0;
  for auto n in 1..N {
    for auto s in 1..S {
      auto x' <- x + q.simulate();
      if simulate_uniform(0.0, 1.0) <= π.pdf(x')/π.pdf(x) {
        x <- x';  // accept
        A <- A + 1;
      }
    }
    X2[n,1..D] <- x;
  }  
  //stderr.print("acceptance rate = " + Real(A)/(N*S) + "\n");
  
  /* test distance between the iid and Metropolis samples */
  if !pass(X1, X2) {
    exit(1);
  }
}
