/**
 * Matrix normal-inverse-gamma distribution.
 */
final class MatrixNormalInverseGamma(M:Expression<Real[_,_]>,
    Σ:Expression<LLT>, σ2:IndependentInverseGamma) <
    Distribution<Real[_,_]> {
  /**
   * Precision.
   */
  Λ:Expression<LLT> <- inv(Σ);

  /**
   * Precision times mean.
   */
  N:Expression<Real[_,_]> <- Λ*M;

  /**
   * Variance shapes.
   */
  α:Expression<Real> <- σ2.α;

  /**
   * Variance scale accumulators.
   */
  γ:Expression<Real[_]> <- σ2.β + 0.5*diagonal(transpose(N)*M);

  /**
   * Variance scales.
   */
  σ2:IndependentInverseGamma& <- σ2;

  function rows() -> Integer {
    return N.rows();
  }
  
  function columns() -> Integer {
    return N.columns();
  }

  function supportsLazy() -> Boolean {
    return true;
  }

  function simulate() -> Real[_,_] {
    return simulate_matrix_normal_inverse_gamma(N.value(), Λ.value(),
        α.value(), gamma_to_beta(γ.value(), N.value(), Λ.value()));
  }

  function simulateLazy() -> Real[_,_]? {
    return simulate_matrix_normal_inverse_gamma(N.get(), Λ.get(),
        α.get(), gamma_to_beta(γ.get(), N.get(), Λ.get()));
  }
  
  function logpdf(X:Real[_,_]) -> Real {
    return logpdf_matrix_normal_inverse_gamma(X, N.value(), Λ.value(),
        α.value(), gamma_to_beta(γ.value(), N.value(), Λ.value()));
  }

  function logpdfLazy(X:Expression<Real[_,_]>) -> Expression<Real>? {
    return logpdf_lazy_matrix_normal_inverse_gamma(X, N, Λ,
        α, gamma_to_beta(γ, N, Λ));
  }

  function update(X:Real[_,_]) {
    auto σ2 <- this.σ2;
    (σ2.α, σ2.β) <- box(update_matrix_normal_inverse_gamma(X, N.value(),
        Λ.value(), α.value(), gamma_to_beta(γ.value(), N.value(),
        Λ.value())));
  }

  function updateLazy(X:Expression<Real[_,_]>) {
    auto σ2 <- this.σ2;
    (σ2.α, σ2.β) <- update_lazy_matrix_normal_inverse_gamma(X, N,
        Λ, α, gamma_to_beta(γ, N, Λ));
  }

  function downdate(X:Real[_,_]) {
    auto σ2 <- this.σ2;
    (σ2.α, σ2.β) <- box(downdate_matrix_normal_inverse_gamma(X, N.value(),
        Λ.value(), α.value(), gamma_to_beta(γ.value(), N.value(),
        Λ.value())));
  }

  function graftMatrixNormalInverseGamma(compare:Distribution<Real[_]>) ->
      MatrixNormalInverseGamma? {
    prune();
    auto σ2 <- this.σ2;
    if σ2 == compare {
      return this;
    } else {
      return nil;
    }
  }

  function link() {
    auto σ2 <- this.σ2;
    σ2.setChild(this);
  }
  
  function unlink() {
    auto σ2 <- this.σ2;
    σ2.releaseChild(this);
  }

  function write(buffer:Buffer) {
    prune();
    buffer.set("class", "MatrixNormalInverseGamma");
    buffer.set("M", solve(Λ.value(), N.value()));
    buffer.set("Σ", inv(Λ.value()));
    buffer.set("α", α);
    buffer.set("β", gamma_to_beta(γ.value(), N.value(), Λ.value()));
  }
}

function MatrixNormalInverseGamma(M:Expression<Real[_,_]>, Σ:Expression<LLT>,
    σ2:IndependentInverseGamma) -> MatrixNormalInverseGamma {
  m:MatrixNormalInverseGamma(M, Σ, σ2);
  m.link();
  return m;
}

/*
 * Compute the variance scaleσ from the variance scale accumulatorσ and other
 * parameters.
 */
function gamma_to_beta(γ:Real[_], N:Real[_,_], Λ:LLT) -> Real[_] {
  return γ - 0.5*diagonal(transpose(N)*solve(Λ, N));
}

function gamma_to_beta(γ:Expression<Real[_]>, N:Expression<Real[_,_]>,
    Λ:Expression<LLT>) -> Expression<Real[_]> {
  return γ - 0.5*diagonal(transpose(N)*solve(Λ, N));
}
