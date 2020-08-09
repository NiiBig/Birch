/**
 * Update the parameters of a Beta distribution with a Bernoulli likelihood.
 *
 * - x: The variate.
 * - α: Prior first shape.
 * - β: Prior second shape.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_beta_bernoulli(x:Expression<Boolean>,
    α:Expression<Real>, β:Expression<Real>) ->
    (Expression<Real>, Expression<Real>) {
  return (α + Real(x), β + (1.0 - Real(x)));
}

/**
 * Update the parameters of a Beta distribution with a Binomial likelihood.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - α: Prior first shape.
 * - β: Prior second shape.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_beta_binomial(x:Expression<Integer>,
    n:Expression<Integer>, α:Expression<Real>, β:Expression<Real>) ->
    (Expression<Real>, Expression<Real>) {
  return (α + Real(x), β + Real(n - x));
}

/**
 * Update the parameters of a Beta distribution with a Negative Binomial likelihood.
 *
 * - x: The variate.
 * - k: Number of successes.
 * - α: Prior first shape.
 * - β: Prior second shape.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_beta_negative_binomial(x:Expression<Integer>,
    k:Expression<Integer>, α:Expression<Real>,
    β:Expression<Real>) -> (Expression<Real>, Expression<Real>) {
  return (α + Real(k), β + Real(x));
}

/**
 * Update the parameters of a Gamma distribution with a Poisson likelihood.
 *
 * - x: The variate.
 * - k: Prior shape.
 * - θ: Prior scale.
 *
 * Returns: the posterior hyperparameters `k'` and `θ'`.
 */
function update_lazy_gamma_poisson(x:Expression<Integer>, k:Expression<Real>,
    θ:Expression<Real>) -> (Expression<Real>, Expression<Real>) {
  return (k + Real(x), θ/(θ + 1.0));
}

/**
 * Update the parameters of a scaled Gamma distribution with a Poisson
 * likelihood.
 *
 * - x: The variate.
 * - a: Scale.
 * - k: Prior shape.
 * - θ: Prior scale.
 *
 * Returns: the posterior hyperparameters `k'` and `θ'`.
 */
function update_lazy_scaled_gamma_poisson(x:Expression<Integer>,
    a:Expression<Real>, k:Expression<Real>, θ:Expression<Real>) ->
    (Expression<Real>, Expression<Real>) {
  return (k + Real(x), θ/(a*θ + 1.0));
}

/**
 * Update the parameters of a Gamma distribution with an exponential
 * likelihood.
 *
 * - x: The variate.
 * - k: Prior shape.
 * - θ: Prior scale.
 *
 * Returns: the posterior hyperparameters `k'` and `θ'`.
 */
function update_lazy_gamma_exponential(x:Expression<Real>,
    k:Expression<Real>, θ:Expression<Real>) ->
    (Expression<Real>, Expression<Real>) {
  return (k + 1.0, θ/(1.0 + x*θ));
}

/**
 * Update the parameters of a scaled Gamma distribution with an exponential
 * likelihood.
 *
 * - x: The variate.
 * - a: Constant scale.
 * - k: Prior shape.
 * - θ: Prior scale.
 *
 * Returns: the posterior hyperparameters `k'` and `θ'`.
 */
function update_lazy_scaled_gamma_exponential(x:Expression<Real>,
    a:Expression<Real>, k:Expression<Real>, θ:Expression<Real>) ->
    (Expression<Real>, Expression<Real>) {
  return (k + 1.0, θ/(1.0 + x*a*θ));
}

/**
 * Update the parameters of an inverse-gamma distribution with a Weibull
 * likelihood with known shape.
 *
 * - x: The variate.
 * - k: Likelihood shape.
 * - α: Prior shape.
 * - β: Prior scale.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_inverse_gamma_weibull(x:Expression<Real>,
    k:Expression<Real>, α:Expression<Real>, β:Expression<Real>) ->
    (Expression<Real>, Expression<Real>) {
  return (α + 1.0, β + pow(x, k));
}

/**
 * Update the parameters of a scaled inverse-gamma distribution with a
 * Weibull likelihood with known shape.
 *
 * - x: The variate.
 * - k: Likelihood shape.
 * - a: Constant scale.
 * - α: Prior shape.
 * - β: Prior scale.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_scaled_inverse_gamma_weibull(x:Expression<Real>,
    k:Expression<Real>, a:Expression<Real>, α:Expression<Real>,
    β:Expression<Real>) -> (Expression<Real>, Expression<Real>) {
  return (α + 1.0, β + pow(x, k)/a);
}

/**
 * Update the parameters of a Dirichlet distribution with a categorical
 * likelihood.
 *
 * - x: The variate.
 * - α: Prior concentrations.
 *
 * Returns: the posterior hyperparameters `α'`.
 */
//function update_lazy_dirichlet_categorical(x:Expression<Integer>,
//    α:Expression<Real[_]>) -> Expression<Real[_]> {
  ///@todo
//}

/**
 * Update the parameters of a Dirichlet distribution with a multinomial
 * likelihood.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - α': Prior concentrations.
 *
 * Returns: the posterior hyperparameters `α'`.
 */
//function update_lazy_dirichlet_multinomial(x:Expression<Integer[_]>,
//    n:Expression<Integer>, α:Expression<Real[_]>) -> Expression<Real[_]> {
  ///@todo
//}

/**
 * Update the parameters of a Gaussian distribution with a Gaussian
 * likelihood.
 *
 * - x: The variate.
 * - μ: Prior mean.
 * - σ2: Prior variance.
 * - s2: Likelihood variance.
 *
 * Returns: the posterior hyperparameters `μ'` and `σ2'`.
 */
function update_lazy_gaussian_gaussian(x:Expression<Real>,
    μ:Expression<Real>, σ2:Expression<Real>, s2:Expression<Real>) ->
    (Expression<Real>, Expression<Real>) {
  auto λ <- 1.0/σ2;
  auto l <- 1.0/s2;
  auto λ' <- λ + l;
  auto μ' <- (λ*μ + l*x)/λ';
  return (μ', 1.0/λ');
}

/**
 * Update the parameters of a Gaussian distribution with a Gaussian
 * likelihood and scaling.
 *
 * - x: The variate.
 * - a: Scale.
 * - μ: Prior mean.
 * - σ2: Prior variance.
 * - c: Offset.
 * - s2: Likelihood variance.
 *
 * Returns: the posterior hyperparameters `μ'` and `λ'`.
 */
function update_lazy_linear_gaussian_gaussian(x:Expression<Real>,
    a:Expression<Real>, μ:Expression<Real>, σ2:Expression<Real>,
    c:Expression<Real>, s2:Expression<Real>) -> (Expression<Real>, Expression<Real>) {
  auto λ <- 1.0/σ2;
  auto l <- 1.0/s2;
  auto λ' <- λ + a*a*l;
  auto μ' <- (λ*μ + a*l*(x - c))/λ';
  return (μ', 1.0/λ');
}

/**
 * Update the parameters of an inverse-gamma distribution that is part
 * of a normal inverse-gamma joint distribution.
 *
 * - x: The variate.
 * - μ: Mean.
 * - λ: Precision.
 * - α: Prior shape of the inverse-gamma.
 * - β: Prior scale of the inverse-gamma.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_normal_inverse_gamma(x:Expression<Real>,
    μ:Expression<Real>, λ:Expression<Real>, α:Expression<Real>,
    β:Expression<Real>) -> (Expression<Real>, Expression<Real>) {
  return (α + 0.5, β + 0.5*pow(x - μ, 2.0)*λ);
}

/**
 * Update the parameters of a normal inverse-gamma distribution with a
 * Gaussian likelihood.
 *
 * - x: The variate.
 * - μ: Mean.
 * - λ: Precision.
 * - α: Prior shape of the inverse-gamma.
 * - β: Prior scale of the inverse-gamma.
 *
 * Returns: the posterior hyperparameters `μ'`, `λ'`, `α'` and `β'`.
 */
function update_lazy_normal_inverse_gamma_gaussian(x:Expression<Real>,
    μ:Expression<Real>, λ:Expression<Real>, α:Expression<Real>,
    β:Expression<Real>) -> (Expression<Real>, Expression<Real>,
    Expression<Real>, Expression<Real>) {
  auto λ' <- λ + 1.0;
  auto μ' <- (λ*μ + x)/λ';
  auto α' <- α + 0.5;
  auto β' <- β + 0.5*(λ/λ')*pow(x - μ, 2.0);
  return (μ', λ', α', β');
}

/**
 * Update the parameters of a normal inverse-gamma distribution with a
 * Gaussian likelihood.
 *
 * - x: The variate.
 * - a: Scale.
 * - μ: Prior mean.
 * - λ: Prior precision.
 * - c: Offset.
 * - α: Prior shape of the inverse-gamma.
 * - β: Prior scale of the inverse-gamma.
 *
 * Returns: the posterior hyperparameters `μ'`, `λ'`, `α'` and `β'`.
 */
function update_lazy_linear_normal_inverse_gamma_gaussian(x:Expression<Real>,
    a:Expression<Real>, μ:Expression<Real>, λ:Expression<Real>,
    c:Expression<Real>, α:Expression<Real>, β:Expression<Real>) ->
    (Expression<Real>, Expression<Real>, Expression<Real>, Expression<Real>) {
  auto y <- x - c;
  auto λ' <- λ + a*a;
  auto μ' <- (λ*μ + a*y)/λ';
  auto α' <- α + 0.5;
  auto β' <- β + 0.5*(y*y + μ*μ*λ - μ'*μ'*λ');
  return (μ', λ', α', β');
}

/**
 * Update the parameters of an inverse-gamma distribution with a
 * gamma likelihood.
 *
 * - x: The variate.
 * - k: Shape of the gamma.
 * - α: Prior shape of the inverse-gamma.
 * - β: Prior scale of the inverse-gamma.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_inverse_gamma_gamma(x:Expression<Real>,
    k:Expression<Real>, α:Expression<Real>, β:Expression<Real>) ->
    (Expression<Real>, Expression<Real>) {
  return (α + k, β + x);
}

/**
 * Update the parameters of a multivariate Gaussian distribution with a
 * multivariate Gaussian likelihood.
 *
 * - x: The variate.
 * - μ: Prior mean.
 * - Σ: Prior covariance.
 * - S: Likelihood covariance.
 *
 * Returns: the posterior hyperparameters `μ'` and `Σ'`.
 */
function update_lazy_multivariate_gaussian_multivariate_gaussian(x:Expression<Real[_]>,
    μ:Expression<Real[_]>, Σ:Expression<LLT>, S:Expression<LLT>) ->
    (Expression<Real[_]>, Expression<LLT>) {
  auto Σ0 <- canonical(Σ);
  auto S0 <- canonical(S);
  auto K' <- transpose(solve(llt(Σ0 + S0), Σ0));
  auto μ' <- μ + K'*(x - μ);
  auto Σ' <- llt(Σ0 - K'*Σ0);
  return (μ', Σ');
}

/**
 * Update the parameters of a multivariate Gaussian distribution with a 
 * linear transformation and multivariate Gaussian likelihood.
 *
 * - x: The variate.
 * - A: Scale.
 * - μ: Prior mean.
 * - Σ: Prior covariance.
 * - c: Offset.
 * - S: Likelihood covariance.
 *
 * Returns: the posterior hyperparameters `μ'` and `Σ'`.
 */
function update_lazy_linear_multivariate_gaussian_multivariate_gaussian(
    x:Expression<Real[_]>, A:Expression<Real[_,_]>, μ:Expression<Real[_]>,
    Σ:Expression<LLT>, c:Expression<Real[_]>, S:Expression<LLT>) ->
    (Expression<Real[_]>, Expression<LLT>) {
  auto Σ0 <- canonical(Σ);
  auto S0 <- canonical(S);
  auto K' <- Σ0*transpose(solve(llt(A*Σ0*transpose(A) + S0), A));
  auto μ' <- μ + K'*(x - A*μ - c);
  auto Σ' <- llt(Σ0 - K'*A*Σ0);
  return (μ', Σ');
}

/**
 * Update the parameters of a multivariate Gaussian distribution with a 
 * linear transformation involving a dot product, and a multivariate Gaussian
 * likelihood.
 *
 * - x: The variate.
 * - a: Scale.
 * - μ: Prior mean.
 * - Σ: Prior covariance.
 * - c: Offset.
 * - s2: Likelihood covariance.
 *
 * Returns: the posterior hyperparameters `μ'` and `Σ'`.
 */
function update_lazy_linear_multivariate_gaussian_gaussian(
    x:Expression<Real>, a:Expression<Real[_]>, μ:Expression<Real[_]>,
    Σ:Expression<LLT>, c:Expression<Real>, s2:Expression<Real>) ->
    (Expression<Real[_]>, Expression<LLT>) {
  auto Σ0 <- canonical(Σ);
  auto k' <- Σ0*a/(dot(a, Σ0*a) + s2);
  auto μ' <- μ + k'*(x - dot(a, μ) - c);
  auto Σ' <- llt(Σ0 - outer(k', a)*Σ0);
  return (μ', Σ');
}

/**
 * Update the parameters of an inverse-gamma distribution with a linear
 * scaling and Gaussian likelihood.
 *
 * - x: The variate.
 * - ν: Precision times mean.
 * - Λ: Precision.
 * - α: Prior shape of the inverse-gamma.
 * - β: Prior scale of the inverse-gamma.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_multivariate_normal_inverse_gamma(x:Expression<Real[_]>,
    ν:Expression<Real[_]>, Λ:Expression<LLT>, α:Expression<Real>,
    β:Expression<Real>) -> (Expression<Real>, Expression<Real>) {
  auto D <- x.length();
  auto μ <- solve(Λ, ν);
  return (α + 0.5*D, β + 0.5*dot(x - μ, canonical(Λ)*(x - μ)));
}

/**
 * Update the parameters of a normal inverse-gamma distribution with a
 * multivariate Gaussian likelihood.
 *
 * - x: The variate.
 * - ν: Prior precision times mean.
 * - Λ: Prior precision.
 * - α: Prior shape of the inverse-gamma.
 * - γ: Prior scale accumulator.
 *
 * Returns: the posterior hyperparameters `μ'`, `Λ'`, `α'` and `γ'`.
 */
function update_lazy_multivariate_normal_inverse_gamma_multivariate_gaussian(
    x:Expression<Real[_]>, ν:Expression<Real[_]>, Λ:Expression<LLT>,
    α:Expression<Real>, γ:Expression<Real>) -> (Expression<Real[_]>,
    Expression<LLT>, Expression<Real>, Expression<Real>) {
  auto D <- x.length();
  auto Λ' <- rank_update(Λ, identity(rows(Λ)));
  auto ν' <- ν + x;
  auto α' <- α + 0.5*D;
  auto γ' <- γ + 0.5*dot(x);
  return (ν', Λ', α', γ');
}

/**
 * Update the parameters of a normal inverse-gamma distribution with a
 * linear transformation and multivariate Gaussian likelihood.
 *
 * - x: The variate.
 * - A: Scale.
 * - ν: Prior precision times mean.
 * - Λ: Prior precision.
 * - c: Offset.
 * - α: Prior shape of the inverse-gamma.
 * - γ: Prior scale accumulator.
 *
 * Returns: the posterior hyperparameters `μ'`, `Λ'`, `γ'`, `α'` and `β'`.
 */
function update_lazy_linear_multivariate_normal_inverse_gamma_multivariate_gaussian(
    x:Expression<Real[_]>, A:Expression<Real[_,_]>, ν:Expression<Real[_]>,
    Λ:Expression<LLT>, c:Expression<Real[_]>, α:Expression<Real>,
    γ:Expression<Real>) -> (Expression<Real[_]>, Expression<LLT>,
    Expression<Real>, Expression<Real>) {
  auto D <- length(x);
  auto Λ' <- rank_update(Λ, transpose(A));
  auto ν' <- ν + transpose(A)*(x - c);
  auto α' <- α + 0.5*D;
  auto γ' <- γ + 0.5*dot(x - c);
  return (ν', Λ', α', γ');
}

/**
 * Update the parameters of a normal inverse-gamma distribution with a
 * linear transformation involving a dot product, and Gaussian likelihood.
 *
 * - x: The variate.
 * - A: Scale.
 * - ν: Prior precision times mean.
 * - Λ: Prior precision.
 * - c: Offset.
 * - α: Prior shape of the inverse-gamma.
 * - γ: Prior scale accumulator.
 *
 * Returns: the posterior hyperparameters `μ'`, `Λ'`, `γ'`, `α'` and `β'`.
 */
function update_lazy_linear_multivariate_normal_inverse_gamma_gaussian(
    x:Expression<Real>, a:Expression<Real[_]>, ν:Expression<Real[_]>,
    Λ:Expression<LLT>, c:Expression<Real>, α:Expression<Real>,
    γ:Expression<Real>) -> (Expression<Real[_]>, Expression<LLT>,
    Expression<Real>, Expression<Real>) {
  auto Λ' <- rank_update(Λ, a);
  auto ν' <- ν + a*(x - c);
  auto α' <- α + 0.5;
  auto γ' <- γ + 0.5*pow(x - c, 2.0);
  return (ν', Λ', α', γ');
}

/**
 * Update the parameters of a matrix normal-inverse-gamma variate.
 *
 * - X: The variate.
 * - N: Precision times mean.
 * - Λ: Precision.
 * - α: Prior variance shape.
 * - β: Prior variance scales.
 *
 * Returns: the posterior hyperparameters `α'` and `β'`.
 */
function update_lazy_matrix_normal_inverse_gamma(X:Expression<Real[_,_]>,
    N:Expression<Real[_,_]>, Λ:Expression<LLT>, α:Expression<Real>,
    β:Expression<Real[_]>) -> (Expression<Real>, Expression<Real[_]>) {
  auto D <- rows(X);
  auto M <- solve(Λ, N);
  auto α' <- α + 0.5*D;
  auto β' <- β + 0.5*diagonal(transpose(X - M)*canonical(Λ)*(X - M));
  return (α', β');
}

/**
 * Update the parameters of a Gaussian variate with
 * matrix-normal-inverse-gamma prior.
 *
 * - x: The variate.
 * - N: Prior precision times mean matrix.
 * - Λ: Prior precision.
 * - α: Prior variance shape.
 * - γ: Prior variance scale accumulators.
 *
 * Returns: the posterior hyperparameters `N'`, `Λ'`, `α'` and `γ'`.
 */
function update_lazy_matrix_normal_inverse_gamma_matrix_gaussian(
    X:Expression<Real[_,_]>, N:Expression<Real[_,_]>, Λ:Expression<LLT>,
    α:Expression<Real>, γ:Expression<Real[_]>) -> (Expression<Real[_,_]>,
    Expression<LLT>, Expression<Real>, Expression<Real[_]>) {
  auto D <- rows(X);
  auto Λ' <- rank_update(Λ, identity(rows(N)));
  auto N' <- N + X;
  auto α' <- α + 0.5*D;
  auto γ' <- γ + 0.5*diagonal(transpose(X)*X);
  return (N', Λ', α', γ');
}

/**
 * Update the parameters of a Gaussian variate with linear transformation
 * of matrix-normal-inverse-gamma prior.
 *
 * - x: The variate.
 * - A: Scale.
 * - N: Prior precision times mean matrix.
 * - Λ: Prior precision.
 * - C: Offset.
 * - α: Prior variance shape.
 * - γ: Prior variance scale accumulators.
 *
 * Returns: the posterior hyperparameters `N'`, `Λ'`, `α'` and `γ'`.
 */
function update_lazy_linear_matrix_normal_inverse_gamma_matrix_gaussian(
    X:Expression<Real[_,_]>, A:Expression<Real[_,_]>,
    N:Expression<Real[_,_]>, Λ:Expression<LLT>, C:Expression<Real[_,_]>,
    α:Expression<Real>, γ:Expression<Real[_]>) -> (Expression<Real[_,_]>,
    Expression<LLT>, Expression<Real>, Expression<Real[_]>) {
  auto D <- rows(X);
  auto Λ' <- rank_update(Λ, transpose(A));
  auto N' <- N + transpose(A)*(X - C);
  auto α' <- α + 0.5*D;
  auto γ' <- γ + 0.5*diagonal(transpose(X - C)*(X - C));
  return (N', Λ', α', γ');
}

/**
 * Update the parameters of a matrix normal-inverse-Wishart variate.
 *
 * - X: The variate.
 * - N: Precision times mean.
 * - Λ: Precision.
 * - V: Prior variance shape.
 * - k: Prior degrees of freedom.
 *
 * Returns: the posterior hyperparameters `V'` and `k'`.
 */
function update_lazy_matrix_normal_inverse_wishart(X:Expression<Real[_,_]>,
    N:Expression<Real[_,_]>, Λ:Expression<LLT>, V:Expression<LLT>,
    k:Expression<Real>) -> (Expression<LLT>, Expression<Real>) {
  auto D <- rows(X);
  auto M <- solve(Λ, N);
  auto V' <- rank_update(V, transpose(X - M));
  auto k' <- k + D;
  return (V', k');
}

/**
 * Update the parameters of a Gaussian variate with
 * matrix-normal-inverse-Wishart prior.
 *
 * - x: The variate.
 * - N: Prior precision times mean matrix.
 * - Λ: Prior precision.
 * - V: Prior variance shape.
 * - k: Prior degrees of freedom.
 *
 * Returns: the posterior hyperparameters `N'`, `Λ'`, `V'` and `k'`.
 */
function update_lazy_matrix_normal_inverse_wishart_matrix_gaussian(
    X:Expression<Real[_,_]>, N:Expression<Real[_,_]>, Λ:Expression<LLT>,
    V:Expression<LLT>, k:Expression<Real>) -> (Expression<Real[_,_]>,
    Expression<LLT>, Expression<LLT>, Expression<Real>) {
  auto D <- rows(X);
  auto Λ' <- rank_update(Λ, identity(rows(N)));
  auto N' <- N + X;
  auto M <- solve(Λ, N);
  auto M' <- solve(Λ', N');
  auto V' <- llt(canonical(V) + transpose(X - M')*(X - M') +
      transpose(M' - M)*canonical(Λ)*(M' - M));
  auto k' <- k + D;
  return (N', Λ', V', k');
}

/**
 * Update the parameters of a Gaussian variate with linear transformation
 * of matrix-normal-inverse-Wishart prior.
 *
 * - x: The variate.
 * - A: Scale.
 * - N: Prior precision times mean matrix.
 * - Λ: Prior precision.
 * - C: Offset.
 * - V: Prior variance shape.
 * - k: Prior degrees of freedom.
 *
 * Returns: the posterior hyperparameters `N'`, `Λ'`, `V'` and `k'`.
 */
function update_lazy_linear_matrix_normal_inverse_wishart_matrix_gaussian(
    X:Expression<Real[_,_]>, A:Expression<Real[_,_]>,
    N:Expression<Real[_,_]>, Λ:Expression<LLT>, C:Expression<Real[_,_]>,
    V:Expression<LLT>, k:Expression<Real>) -> (Expression<Real[_,_]>,
    Expression<LLT>, Expression<LLT>, Expression<Real>) {
  auto D <- rows(X);
  auto Λ' <- rank_update(Λ, transpose(A));
  auto N' <- N + transpose(A)*(X - C);
  auto M <- solve(Λ, N);
  auto M' <- solve(Λ', N');
  auto V' <- llt(canonical(V) + transpose(X - A*M' - C)*(X - A*M' - C) +
      transpose(M' - M)*canonical(Λ)*(M' - M));
  auto k' <- k + D;
  return (N', Λ', V', k');
}
