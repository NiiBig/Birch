class TestLinearMatrixNormalInverseGammaMatrixGaussian < Model {
  σ2:Random<Real[_]>;
  X:Random<Real[_,_]>;
  Y:Random<Real[_,_]>;

  n:Integer <- 5;
  p:Integer <- 2;
  α:Real;
  A:Real[n,n];
  M:Real[n,p];
  Σ:Real[n,n];
  C:Real[n,p];
  β:Real[p];

  function initialize() {
    α <- simulate_uniform(2.0, 10.0);
    for i in 1..n {
      for j in 1..n {
        A[i,j] <- simulate_uniform(-2.0, 2.0);
        Σ[i,j] <- simulate_uniform(-2.0, 2.0);
      }
      for j in 1..p {
        M[i,j] <- simulate_uniform(-10.0, 10.0);
        C[i,j] <- simulate_uniform(-10.0, 10.0);
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
    Y ~ Gaussian(A*X + C, σ2);
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
