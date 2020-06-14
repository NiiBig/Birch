/**
 * Matrix Gaussian distribution where each column is independent.
 */
final class IndependentColumnMatrixGaussian(M:Expression<Real[_,_]>,
    U:Expression<LLT>, v:Expression<Real[_]>) < Distribution<Real[_,_]> {
  /**
   * Mean.
   */
  M:Expression<Real[_,_]> <- M;
  
  /**
   * Among-row covariance.
   */
  U:Expression<LLT> <- U;

  /**
   * Among-column variances.
   */
  σ2:Expression<Real[_]> <- v;

  function rows() -> Integer {
    return M.rows();
  }

  function columns() -> Integer {
    return M.columns();
  }

  function supportsLazy() -> Boolean {
    return true;
  }

  function simulate() -> Real[_,_] {
    return simulate_matrix_gaussian(M.value(), U.value(), σ2.value());
  }

  function simulateLazy() -> Real[_,_]? {
    return simulate_matrix_gaussian(M.get(), U.get(), σ2.get());
  }
  
  function logpdf(x:Real[_,_]) -> Real {
    return logpdf_matrix_gaussian(x, M.value(), U.value(), σ2.value());
  }

  function logpdfLazy(x:Expression<Real[_,_]>) -> Expression<Real>? {
    return logpdf_lazy_matrix_gaussian(x, M, U, σ2);
  }

  function graft() -> Distribution<Real[_,_]> {
    prune();
    s1:IndependentInverseGamma?;
    r:Distribution<Real[_,_]> <- this;
    
    /* match a template */
    if (s1 <- σ2.graftIndependentInverseGamma())? {
      r <- MatrixNormalInverseGamma(M, U, s1!);
    } else {
      r <- Gaussian(M, U, diagonal(σ2));
    }
    
    return r;
  }

  function graftMatrixGaussian() -> MatrixGaussian? {
    prune();
    return Gaussian(M, U, diagonal(σ2));
  }

  function graftMatrixNormalInverseGamma(compare:Distribution<Real[_]>) ->
      MatrixNormalInverseGamma? {
    prune();
    s1:IndependentInverseGamma?;
    r:MatrixNormalInverseGamma?;
    
    /* match a template */
    if (s1 <- σ2.graftIndependentInverseGamma())? && s1! == compare {
      r <- MatrixNormalInverseGamma(M, U, s1!);
    }

    return r;
  }
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Expression<Real[_,_]>, U:Expression<LLT>,
    σ2:Expression<Real[_]>) -> IndependentColumnMatrixGaussian {
  m:IndependentColumnMatrixGaussian(M, U, σ2);
  return m;
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Expression<Real[_,_]>, U:Expression<LLT>, σ2:Real[_]) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(M, U, box(σ2));
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Expression<Real[_,_]>, U:LLT, σ2:Expression<Real[_]>) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(M, box(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Expression<Real[_,_]>, U:LLT, σ2:Real[_]) ->
      IndependentColumnMatrixGaussian {
  return Gaussian(M, box(U), box(σ2));
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Real[_,_], U:Expression<LLT>, σ2:Expression<Real[_]>) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(box(M), U, σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Real[_,_], U:Expression<LLT>, σ2:Real[_]) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(box(M), U, box(σ2));
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Real[_,_], U:LLT, σ2:Expression<Real[_]>) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(box(M), box(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Real[_,_], U:LLT, σ2:Real[_]) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(box(M), box(U), box(σ2));
}




/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Expression<Real[_,_]>, U:Expression<Real[_,_]>,
    σ2:Expression<Real[_]>) -> IndependentColumnMatrixGaussian {
  return Gaussian(M, llt(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Expression<Real[_,_]>, U:Expression<Real[_,_]>,
    σ2:Real[_]) -> IndependentColumnMatrixGaussian {
  return Gaussian(M, llt(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Expression<Real[_,_]>, U:Real[_,_],
    σ2:Expression<Real[_]>) -> IndependentColumnMatrixGaussian {
  return Gaussian(M, llt(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Expression<Real[_,_]>, U:Real[_,_], σ2:Real[_]) ->
      IndependentColumnMatrixGaussian {
  return Gaussian(M, llt(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Real[_,_], U:Expression<Real[_,_]>,
    σ2:Expression<Real[_]>) -> IndependentColumnMatrixGaussian {
  return Gaussian(M, llt(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Real[_,_], U:Expression<Real[_,_]>, σ2:Real[_]) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(M, llt(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Real[_,_], U:Real[_,_], σ2:Expression<Real[_]>) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(M, llt(U), σ2);
}

/**
 * Create matrix Gaussian distribution where each column is independent.
 */
function Gaussian(M:Real[_,_], U:Real[_,_], σ2:Real[_]) ->
    IndependentColumnMatrixGaussian {
  return Gaussian(M, llt(U), σ2);
}
