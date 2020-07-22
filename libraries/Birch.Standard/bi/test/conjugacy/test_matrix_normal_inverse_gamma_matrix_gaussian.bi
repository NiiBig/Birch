/*
 * Test matrix normal-inverse-gamma-Gaussian conjugacy.
 */
program test_matrix_normal_inverse_gamma_matrix_gaussian(N:Integer <- 10000) {
  m:TestMatrixNormalInverseGammaMatrixGaussian;
  test_conjugacy(m, N, m.size());
}

class TestMatrixNormalInverseGammaMatrixGaussian < Model {
  σ2:Random<Real[_]>;
  X:Random<Real[_,_]>;
  Y:Random<Real[_,_]>;

  n:Integer <- 5;
  p:Integer <- 2;
  M:Real[n,p];
  Σ:Real[n,n];
  α:Real;
  β:Real[p];

  function initialize() {
    α <- simulate_uniform(2.0, 10.0);
    for i in 1..n {
      for j in 1..n {
        Σ[i,j] <- simulate_uniform(-2.0, 2.0);
      }
      for j in 1..p {
        M[i,j] <- simulate_uniform(-10.0, 10.0);
      }
    }
    for i in 1..p {
      β[i] <- simulate_uniform(0.0, 10.0);
    }
    Σ <- Σ*transpose(Σ);
  }
  
  fiber simulate() -> Event {
    σ2 ~ InverseGamma(α, β);
    X ~ Gaussian(M, Σ, σ2);
    Y ~ Gaussian(X, σ2);
  }
  
  function forward() -> Real[_] {
    assert !σ2.hasValue();
    σ2.value();
    assert !X.hasValue();
    X.value();
    assert !Y.hasValue();
    Y.value();
    return vectorize();
  }

  function backward() -> Real[_] {
    assert !Y.hasValue();
    Y.value();
    assert !X.hasValue();
    X.value();
    assert !σ2.hasValue();
    σ2.value();
    return vectorize();
  }
  
  function marginal() -> Distribution<Real[_,_]> {
    return Y.distribution()!;
  }
  
  function vectorize() -> Real[_] {
    y:Real[size()];
    y[1..length(σ2)] <- σ2.value();
    auto k <- length(σ2);
    for i in 1..rows(X) {
      y[k + 1 .. k + columns(X)] <- X.value()[i,1..columns(X)];
      k <- k + columns(X);
    }
    for i in 1..rows(Y) {
      y[k + 1 .. k + columns(Y)] <- Y.value()[i,1..columns(Y)];
      k <- k + columns(Y);
    }    
    return y;
  }
  
  function size() -> Integer {
    return p + 2*n*p;
  }
}
