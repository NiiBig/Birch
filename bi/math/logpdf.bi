/**
 * Observe a Bernoulli variate.
 *
 * - x: The variate.
 * - ρ: Probability of a true result.
 *
 * Returns: the log probability mass.
 */
function logpdf_bernoulli(x:Boolean, ρ:Real) -> Real {
  assert 0.0 <= ρ && ρ <= 1.0;
  if (x) {
    return log(ρ);
  } else {
    return log1p(-ρ);
  }
}

/**
 * Observe a delta variate.
 *
 * - x: The variate.
 * - μ: Location.
 *
 * Returns: the log probability mass.
 */
function logpdf_delta(x:Integer, μ:Integer) -> Real {
  if (x == μ) {
    return 0.0;
  } else {
    return -inf;
  }
}

/**
 * Observe a binomial variate.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - ρ: Probability of a true result.
 *
 * Returns: the log probability mass.
 */
function logpdf_binomial(x:Integer, n:Integer, ρ:Real) -> Real {
  assert 0 <= n;
  assert 0.0 <= ρ && ρ <= 1.0;

  if ρ == 0.0 || ρ == 1.0 {
    if x == n*ρ {
      return 0.0;
    } else {
      return -inf;
    }
  } else if 0 <= x && x <= n {
    return x*log(ρ) + (n - x)*log1p(-ρ) + lchoose(n, x);
  } else {
    return -inf;
  }
}

/**
 * Observe a negative binomial variate.
 *
 * - x: The variate (number of failures).
 * - k: Number of successes before the experiment is stopped.
 * - ρ: Probability of success.
 *
 * Returns: the log probability mass.
 */
function logpdf_negative_binomial(x:Integer, k:Integer, ρ:Real) -> Real {
  assert 0 < k;
  assert 0.0 <= ρ && ρ <= 1.0;

  if (x >= 0) {
    return k*log(ρ) + x*log1p(-ρ) + lchoose(x + k - 1, x);
  } else {
    return -inf;
  }
}

/**
 * Observe a Poisson variate.
 *
 * - x: The variate.
 * - λ: Rate.
 *
 * Returns: the log probability mass.
 */
function logpdf_poisson(x:Integer, λ:Real) -> Real {
  assert 0.0 <= λ;

  if (λ > 0.0) {
    if (x >= 0) {
      return x*log(λ) - λ - lgamma(x + 1);
    } else {
      return -inf;
    }
  } else {
    if (x == 0) {
      return inf;
    } else {
      return -inf;
    }
  }
}

/**
 * Observe an integer uniform variate.
 *
 * - x: The variate.
 * - l: Lower bound of interval.
 * - u: Upper bound of interval.
 *
 * Returns: the log probability mass.
 */
function logpdf_uniform_int(x:Integer, l:Integer, u:Integer) -> Real {
  if (x >= l && x <= u) {
    return -log(u - l + 1);
  } else {
    return -inf;
  }
}

/**
 * Observe a categorical variate.
 *
 * - x: The variate.
 * - ρ: Category probabilities.
 *
 * Returns: the log probability mass.
 */
function logpdf_categorical(x:Integer, ρ:Real[_]) -> Real {
  if (1 <= x && x <= length(ρ)) {
    assert ρ[x] >= 0.0;
    return log(ρ[x]);
  } else {
    return -inf;
  }
}

/**
 * Observe a multinomial variate.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - ρ: Category probabilities.
 *
 * Returns: the log probability mass.
 */
function logpdf_multinomial(x:Integer[_], n:Integer, ρ:Real[_]) -> Real {
  assert length(x) == length(ρ);

  m:Integer <- 0;
  w:Real <- lgamma(n + 1.0);
  for (i:Integer in 1..length(x)) {
    assert x[i] >= 0;
    assert ρ[i] >= 0.0;
    m <- m + x[i];
    w <- w + x[i]*log(ρ[i]) - lgamma(x[i] + 1.0);
  }
  if (m == n) {
    return w;
  } else {
    return -inf;
  }
}

/**
 * Observe a Dirichlet variate.
 *
 * - x: The variate.
 * - α: Concentrations.
 *
 * Returns: the log probability density.
 */
function logpdf_dirichlet(x:Real[_], α:Real[_]) -> Real {
  assert length(x) == length(α);

  D:Integer <- length(x);
  w:Real <- 0.0;
  for (i:Integer in 1..D) {
    assert x[i] >= 0.0;
    w <- w + (α[i] - 1.0)*log(x[i]) - lgamma(α[i]);
  }
  w <- w + lgamma(sum(α)); 
  return w;
}

/**
 * Observe a uniform variate.
 *
 * - x: The variate.
 * - l: Lower bound of interval.
 * - u: Upper bound of interval.
 *
 * Returns: the log probability density.
 */
function logpdf_uniform(x:Real, l:Real, u:Real) -> Real {
  assert l <= u;

  if (x >= l && x <= u) {
    return -log(u - l);
  } else {
    return -inf;
  }
}

/**
 * Observe an exponential variate.
 *
 * - x: The variate.
 * - λ: Rate.
 *
 * Returns: the log probability density.
 */
function logpdf_exponential(x:Real, λ:Real) -> Real {
  assert 0.0 < λ;

  if (x >= 0.0) {
    return log(λ) - λ*x;
  } else {
    return -inf;
  }
}

/**
 * Observe a Weibull variate.
 *
 * - x: The variate.
 * - k: Shape.
 * - λ: Scale.
 *
 * Returns: the log probability density.
 */
function logpdf_weibull(x:Real, k:Real, λ:Real) -> Real {
  assert 0.0 < λ;

  if (x >= 0.0) {
    return log(k) + (k - 1.0)*log(x) - (k - 2.0)*log(λ) - pow(x/λ, k);
  } else {
    return -inf;
  }
}

/**
 * Observe a Gaussian variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - σ2: Variance.
 *
 * Returns: the log probability density.
 */
function logpdf_gaussian(x:Real, μ:Real, σ2:Real) -> Real {
  assert 0.0 <= σ2;
  
  if (σ2 == 0.0) {
    if (x == μ) {
      return inf;
    } else {
      return -inf;
    }
  } else {
    return -0.5*(pow(x - μ, 2.0)/σ2 + log(2.0*π*σ2));
  }
}

/**
 * Observe a log-Gaussian variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - σ2: Variance.
 *
 * Returns: the log probability density.
 */
function logpdf_log_gaussian(x:Real, μ:Real, σ2:Real) -> Real {
  if (x > 0.0) {
    return logpdf_gaussian(log(x), μ, σ2) - log(x);
  } else {
    return -inf;
  }
}

/**
 * Observe a Student's $t$ variate.
 *
 * - x: The variate.
 * - ν: Degrees of freedom.
 *
 * Returns: the log probability density.
 */
function logpdf_student_t(x:Real, ν:Real) -> Real {
  assert 0.0 < ν;
  z:Real <- 0.5*(ν + 1.0);
  return lgamma(z) - lgamma(0.5*ν) - z*log1p(x*x/ν) - 0.5*log(π*ν);
}

/**
 * Observe a Student's $t$ variate with location and scale.
 *
 * - x: The variate.
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - σ2: Squared scale.
 *
 * Returns: the log probability density.
 */
function logpdf_student_t(x:Real, ν:Real, μ:Real, σ2:Real) -> Real {
  assert 0.0 < ν;
  assert 0.0 < σ2;
  return logpdf_student_t((x - μ)/sqrt(σ2), ν) - 0.5*log(σ2);
}

/**
 * Observe a beta variate.
 *
 * - x: The variate.
 * - α: Shape.
 * - β: Shape.
 *
 * Returns: the log probability density.
 */
function logpdf_beta(x:Real, α:Real, β:Real) -> Real {
  assert 0.0 < α;
  assert 0.0 < β;

  if (0.0 < x && x < 1.0) {
    return (α - 1.0)*log(x) + (β - 1.0)*log1p(-x) - lbeta(α, β);
  } else {
    return -inf;
  }
}

/**
 * Observe a gamma variate.
 *
 * - x: The variate.
 * - k: Shape.
 * - θ: Scale.
 *
 * Returns: the log probability density.
 */
function logpdf_gamma(x:Real, k:Real, θ:Real) -> Real {
  assert 0.0 < k;
  assert 0.0 < θ;
  
  if (x > 0.0) {
    return (k - 1.0)*log(x) - x/θ - lgamma(k) - k*log(θ);
  } else {
    return -inf;
  }
}

/**
 * Observe an inverse-gamma variate.
 *
 * - x: The variate.
 * - α: Shape.
 * - β: Scale.
 *
 * Returns: the log probability density.
 */
function logpdf_inverse_gamma(x:Real, α:Real, β:Real) -> Real {
  assert 0.0 < α;
  assert 0.0 < β;
  
  if (x > 0.0) {
    return α*log(β) - (α + 1.0)*log(x) - β/x - lgamma(α);
  } else {
    return -inf;
  }
}

/**
 * Observe a compound-gamma variate.
 *
 * - x: The variate.
 * - k: Shape.
 * - α: Prior shape.
 * - β: Prior scale.
 *
 * Return: the log probability density.
 */
function logpdf_compound_gamma(x:Real, k:Real, α:Real, β:Real) -> Real {
  assert 0.0 < k;
  assert 0.0 < α;
  assert 0.0 < β;

  if x > 0.0 {
    return (k - 1)*log(x) + α*log(β) - (α + k)*log(β + x) - lbeta(α, k);
  } else {
    return -inf;
  }
}

/**
 * Observe a normal inverse-gamma variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - a2: Variance.
 * - α: Shape of inverse-gamma on scale.
 * - β: Scale of inverse-gamma on scale.
 *
 * Returns: the log probability density.
 */
function logpdf_normal_inverse_gamma(x:Real, μ:Real, a2:Real, α:Real,
    β:Real) -> Real {
  return logpdf_student_t(x, 2.0*α, μ, a2*β/α);
}

/**
 * Observe a beta-bernoulli variate.
 *
 * - x: The variate.
 * - α: Shape.
 * - β: Shape.
 *
 * Returns: the log probability mass.
 */
function logpdf_beta_bernoulli(x:Boolean, α:Real, β:Real) -> Real {
  assert 0.0 < α;
  assert 0.0 < β;

  if (x) {
    return lbeta(1.0 + α, β) - lbeta(α, β);
  } else {
    return lbeta(α, 1.0 + β) - lbeta(α, β);
  }
}

/**
 * Observe a beta-binomial variate.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - α: Shape.
 * - β: Shape.
 *
 * Returns: the log probability mass.
 */
function logpdf_beta_binomial(x:Integer, n:Integer, α:Real, β:Real) -> Real {
  assert 0 <= n;
  assert 0.0 < α;
  assert 0.0 < β;

  if (0 <= x && x <= n) {
    return lbeta(x + α, n - x + β) - lbeta(α, β) + lchoose(n, x);
  } else {
    return -inf;
  }
}

/**
 * Observe a beta-negative-binomial variate
 *
 * - x: The variate.
 * - k: Number of successes.
 * - α: Shape.
 * - β: Shape.
 *
 * Returns: the log probability mass.
 */
function logpdf_beta_negative_binomial(x:Integer, k:Integer, α:Real, β:Real) -> Real {
  assert 0.0 < α;
  assert 0.0 < β;

  if (x >= 0) {
    return lbeta(α + k, β + x) - lbeta(α, β) + lchoose(x + k - 1, x);
  } else {
    return -inf;
  }
}

/**
 * Observe a gamma-Poisson variate.
 *
 * - x: The variate.
 * - k: Shape.
 * - θ: Scale.
 *
 * Returns: the log probability mass.
 */
function logpdf_gamma_poisson(x:Integer, k:Real, θ:Real) -> Real {
  assert 0.0 < k;
  assert 0.0 < θ;
  assert k == floor(k);

  return logpdf_negative_binomial(x, Integer(k), 1.0/(θ + 1.0));
}

/**
 * Observe of a Lomax variate.
 *
 * - x: The variate.
 * - λ: Scale.
 * - α: Shape.
 *
 * Return: the log probability density.
 */
function logpdf_lomax(x:Real, λ:Real, α:Real) -> Real {
  assert 0.0 < λ;
  assert 0.0 < α;
  if x >= 0.0 {
    return log(α) - log(λ) - (α + 1.0)*log1p(x/λ);
  } else {
    return -inf;
  }
}

/**
 * Observe a Dirichlet-categorical variate.
 *
 * - x: The variate.
 * - α: Concentrations.
 *
 * Returns: the log probability mass.
 */
function logpdf_dirichlet_categorical(x:Integer, α:Real[_]) -> Real {
  if (1 <= x && x <= length(α)) {
    A:Real <- sum(α);
    return lgamma(1.0 + α[x]) - lgamma(α[x]) + lgamma(A) - lgamma(1.0 + A);
  } else {
    return -inf;
  }
}

/**
 * Observe a Dirichlet-multinomial variate.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - α: Concentrations.
 *
 * Returns: the log probability mass.
 */
function logpdf_dirichlet_multinomial(x:Integer[_], n:Integer, α:Real[_]) -> Real {
  assert length(x) == length(α);

  A:Real <- sum(α);
  m:Integer <- 0;
  w:Real <- lgamma(n + 1.0) + lgamma(A) - lgamma(n + A);
  for (i:Integer in 1..length(α)) {
    assert x[i] >= 0;
    m <- m + x[i];
    w <- w + lgamma(x[i] + α[i]) - lgamma(x[i] + 1.0) - lgamma(α[i]);
  }
  if (m == n) {
    return w;
  } else {
    return -inf;
  }
}

/**
 * Observe a categorical variate with Chinese restaurant process
 * prior.
 *
 * - x: The variate.
 * - α: Concentration.
 * - θ: Discount.
 * - n: Enumerated items.
 * - N: Total number of items.
 */
function logpdf_crp_categorical(k:Integer, α:Real, θ:Real,
    n:Integer[_], N:Integer) -> Real {
  K:Integer <- length(n);
  if (k > K + 1) {
    return -inf;
  } else if (k == K + 1) {
    return log(K*α + θ) - log(N + θ);
  } else {
    return log(n[k] - α) - log(N + θ);
  }
}

/**
 * Observe a Gaussian variate with an inverse-gamma distribution over
 * the variance.
 *
 * - x: The variate.
 * - μ: Mean.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the log probability density.
 */
function logpdf_inverse_gamma_gaussian(x:Real, μ:Real, α:Real,
    β:Real) -> Real {
  return logpdf_student_t(x, 2.0*α, μ, β/α);
}

/**
 * Observe a Gaussian variate with a normal inverse-gamma prior.
 *
 * - x: The variate.
 * - μ: Mean.
 * - a2: Variance.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the log probability density.
 */
function logpdf_normal_inverse_gamma_gaussian(x:Real, μ:Real, a2:Real,
    α:Real, β:Real) -> Real {
  return logpdf_student_t(x, 2.0*α, μ, (β/α)*(1.0 + a2));
}

/**
 * Observe a Gaussian variate with a normal inverse-gamma prior with linear
 * transformation.
 *
 * - x: The variate.
 * - a: Scale.
 * - μ: Mean.
 * - c: Offset.
 * - a2: Variance.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the log probability density.
 */
function logpdf_linear_normal_inverse_gamma_gaussian(x:Real, a:Real,
    μ:Real, c:Real, a2:Real, α:Real, β:Real) -> Real {
  return logpdf_student_t(x, 2.0*α, a*μ + c, (β/α)*(1.0 + a*a*a2));
}

/**
 * Observe a multivariate Gaussian variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - Σ: Covariance.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_gaussian(x:Real[_], μ:Real[_], Σ:Real[_,_]) ->
    Real {
  D:Integer <- length(μ);
  return -0.5*(dot(x - μ, cholsolve(Σ, x - μ)) + D*log(2.0*π) + lcholdet(Σ));
}

/**
 * Observe a multivariate Gaussian distribution with diagonal covariance.
 *
 * - x: The variate.
 * - μ: Mean.
 * - σ2: Variance.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_gaussian(x:Real[_], μ:Real[_], σ2:Real) -> Real {
  D:Integer <- length(μ);
  return -0.5*(dot(x - μ)/σ2 + D*log(2.0*π*σ2));
}

/**
 * Observe a multivariate Student's $t$-distribution variate with location
 * and scale.
 *
 * - x: The variate.
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - Λ: Precision.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_student_t(x:Real[_], ν:Real, μ:Real[_],
    Λ:Real[_,_]) -> Real {
  D:Integer <- length(μ);
  z:Real <- 0.5*(ν + D);
  return lgamma(z) - lgamma(0.5*ν) - z*log1p(dot(x - μ, Λ*(x - μ))/ν) - 0.5*D*log(π*ν) + 0.5*lcholdet(Λ);
}

/**
 * Observe a multivariate Student's $t$-distribution variate with location
 * and diagonal scale.
 *
 * - x: The variate.
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - λ: Precision.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_student_t(x:Real[_], ν:Real, μ:Real[_],
    λ:Real) -> Real {
  D:Integer <- length(μ);
  z:Real <- 0.5*(ν + D);
  return lgamma(z) - lgamma(0.5*ν) - z*log1p(dot(x - μ)*λ/ν) - 0.5*D*log(π*ν) + 0.5*D*log(λ);
}

/**
 * Observe a multivariate normal inverse-gamma variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - Λ: Precision.
 * - α: Shape of inverse-gamma on scale.
 * - β: Scale of inverse-gamma on scale.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_normal_inverse_gamma(x:Real[_], μ:Real[_],
    Λ:LLT, α:Real, β:Real) -> Real {
  return logpdf_multivariate_student_t(x, 2.0*α, μ, Λ*(α/β));
}

/**
 * Observe a multivariate Gaussian variate with an inverse-gamma distribution
 * over a diagonal covariance.
 *
 * - x: The variate.
 * - μ: Mean.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_inverse_gamma_gaussian(x:Real[_], μ:Real[_],
    α:Real, β:Real) -> Real {
  return logpdf_multivariate_student_t(x, 2.0*α, μ, β/α);
}

/**
 * Observe a multivariate Gaussian variate with a multivariate normal
 * inverse-gamma prior.
 *
 * - x: The variate.
 * - μ: Mean.
 * - Λ: Precision.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_normal_inverse_gamma_gaussian(x:Real[_],
    μ:Real[_], Λ:LLT, α:Real, β:Real) -> Real {
  D:Integer <- length(μ);
  return logpdf_multivariate_student_t(x, 2.0*α, μ, (α/β)*inv(identity(D) + inv(Λ)));
}

/**
 * Observe a multivariate Gaussian variate with a multivariate linear normal
 * inverse-gamma prior with linear transformation.
 *
 * - x: The variate.
 * - A: Scale.
 * - μ: Mean.
 * - c: Offset.
 * - Λ: Precision.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_linear_normal_inverse_gamma_gaussian(x:Real[_],
    A:Real[_,_], μ:Real[_], c:Real[_], Λ:LLT, α:Real, β:Real) -> Real {
  return logpdf_multivariate_student_t(x, 2.0*α, A*μ + c,
      (α/β)*inv(identity(rows(A)) + A*solve(Λ, transpose(A))));
}

/**
 * Observe a multivariate Gaussian variate with a multivariate dot normal
 * inverse-gamma prior with linear transformation.
 *
 * - x: The variate.
 * - a: Scale.
 * - μ: Mean.
 * - c: Offset.
 * - Λ: Precision.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_dot_normal_inverse_gamma_gaussian(x:Real,
    a:Real[_], μ:Real[_], c:Real, Λ:LLT, α:Real, β:Real) -> Real {
  return logpdf_student_t(x, 2.0*α, dot(a, μ) + c,
      (β/α)*(1.0 + dot(a, solve(Λ, a))));
}

/**
 * Observe a multivariate uniform distribution.
 *
 * - x: The variate.
 * - l: Lower bound of hyperrectangle.
 * - u: Upper bound of hyperrectangle.
 *
 * Returns: the log probability density.
 */
function logpdf_multivariate_uniform(x:Real[_], l:Real[_], u:Real[_]) -> Real {
  assert length(x) > 0;
  assert length(l) == length(x);
  assert length(u) == length(x);

  D:Integer <- length(l);
  w:Real <- 0.0;
  for (d:Integer in 1..D) {
    w <- w + logpdf_uniform(x[d], l[d], u[d]);
  }
  return w;
}

/**
 * Observe a multivariate uniform distribution over integers.
 *
 * - x: The variate.
 * - l: Lower bound of hyperrectangle.
 * - u: Upper bound of hyperrectangle.
 *
 * Returns: the log probability mass.
 */
function logpdf_multivariate_uniform_int(x:Integer[_], l:Integer[_], u:Integer[_]) -> Real {
  assert length(x) > 0;
  assert length(l) == length(x);
  assert length(u) == length(x);
  
  D:Integer <- length(x);
  w:Real <- 0.0;
  for (d:Integer in 1..D) {
    w <- w + logpdf_uniform_int(x[d], l[d], u[d]);
  }
  return w;
}

/**
 * Observe ridge regression parameters.
 *
 * - W: Weight matrix.
 * - σ2: Variance vector.
 * - N: Prior precision times mean for weights, where each column represents
 *      the mean of the weight for a separate output. 
 * - Λ: Common prior precision.
 * - α: Common prior weight and likelihood covariance shape.
 * - β: Prior covariance scale accumulators.
 *
 * Returns: Matrix of weights and vector of variances, where each column in
 * the matrix and element in the vector corresponds to a different output
 * in the regression.
 */
function logpdf_ridge(W:Real[_,_], σ2:Real[_], N:Real[_,_], Λ:LLT, α:Real,
    γ:Real[_]) -> Real {
  auto R <- rows(N);
  auto C <- columns(N);
  auto M <- solve(Λ, N);
  auto Σ <- inv(Λ);
  auto β <- γ - 0.5*diagonal(transpose(N)*M);
  
  w:Real <- 0.0;
  for auto j in 1..C {
    w <- w + logpdf_inverse_gamma(σ2[j], α, β[j]);
    w <- w + logpdf_multivariate_gaussian(W[1..R,j], M[1..R,j], Σ*σ2[j]);
  }    
  return w;
}

/**
 * Observe regression.
 *
 * - x: Outputs of the regression.
 * - W: Weight matrix, where each column represents the weights for a
 *      different output. 
 * - σ2: Variance vector, where each element represents the variance for a
 *      different output.
 * - u: Input.
 *
 * Returns: the log probability density.
 */
function logpdf_regression(x:Real[_], W:Real[_,_], σ2:Real[_], u:Real[_]) ->
    Real {
  auto μ <- transpose(W)*u;
  auto D <- length(μ);
  auto w <- 0.0;
  for auto d in 1..D {
    w <- w + logpdf_gaussian(x[d], μ[d], σ2[d]);
  }
  return w;
}

/**
 * Observe a ridge regression.
 *
 * - x: The variate.
 * - N: Prior precision times mean for weights, where each column represents
 *      the mean of the weight for a separate output. 
 * - Λ: Common prior precision.
 * - α: Common prior weight and likelihood covariance shape.
 * - β: Prior covariance scale accumulators.
 * - u: Input.
 *
 * Returns: the log probability density.
 */
function logpdf_ridge_regression(x:Real[_], N:Real[_,_], Λ:LLT, α:Real,
    γ:Real[_], u:Real[_]) -> Real {
  D:Integer <- columns(N);
  M:Real[_,_] <- solve(Λ, N);
  μ:Real[_] <- transpose(M)*u;
  σ2:Real[_] <- (γ - 0.5*diagonal(transpose(N)*M))*(1.0 + dot(u, solve(Λ, u)))/α;  
  w:Real <- 0.0;
  for d:Integer in 1..D {
    w <- w + logpdf_student_t(x[d], 2.0*α, μ[d], σ2[d]);
  }
  return w;
}
