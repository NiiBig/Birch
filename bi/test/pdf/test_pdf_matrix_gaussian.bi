/*
 * Test matrix Gaussian pdf evaluations.
 */
program test_pdf_matrix_gaussian(R:Integer <- 4, C:Integer <- 3,
    N:Integer <- 10000, B:Integer <- 500, S:Integer <- 50) {
  M:Real[R,C];
  U:Real[R,R];
  V:Real[C,C];

  for auto i in 1..R {
    for auto j in 1..C {
      M[i,j] <- simulate_uniform(-10.0, 10.0);
    }
  }
  for auto i in 1..R {
    for auto j in 1..R {
      U[i,j] <- simulate_uniform(-2.0, 2.0);
    }
  }
  for auto i in 1..C {
    for auto j in 1..C {
      V[i,j] <- simulate_uniform(-2.0, 2.0);
    }
  }
  U <- U*transpose(U);
  V <- V*transpose(V);

  auto π <- Gaussian(M, U, V);
  test_pdf(π, R, C, N, B, S);
}
