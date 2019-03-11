cpp{{
#include <random>

extern std::random_device rd;
#pragma omp threadprivate(rd)
std::random_device rd;

extern std::mt19937_64 rng;
#pragma omp threadprivate(rng)
std::mt19937_64 rng(rd());
}}

/**
 * Seed the pseudorandom number generator.
 *
 * - seed: Seed value.
 */
function seed(s:Integer) {
  cpp{{
  #ifdef _OPENMP
  #pragma omp parallel num_threads(libbirch::nthreads)
  {
    rng.seed(s + libbirch::tid);
  }
  #else
  rng.seed(s);
  #endif
  }}
}

/**
 * Simulate a Bernoulli distribution.
 *
 * - ρ: Probability of a true result.
 */
function simulate_bernoulli(ρ:Real) -> Boolean {
  assert 0.0 <= ρ && ρ <= 1.0;
  cpp{{
  return std::bernoulli_distribution(ρ)(rng);
  }}
}

/**
 * Simulate a delta distribution.
 *
 * - μ: Location.
 */
function simulate_delta(μ:Integer) -> Integer {
  return μ;
}

/**
 * Simulate a binomial distribution.
 *
 * - n: Number of trials.
 * - ρ: Probability of a true result.
 */
function simulate_binomial(n:Integer, ρ:Real) -> Integer {
  assert 0 <= n;
  assert 0.0 <= ρ && ρ <= 1.0;
  cpp{{
  return std::binomial_distribution<bi::type::Integer>(n, ρ)(rng);
  }}
}

/**
 * Simulate a negative binomial distribution.
 *
 * - k: Number of successes before the experiment is stopped.
 * - ρ: Probability of success.
 *
 * Returns the number of failures.
 */
function simulate_negative_binomial(k:Integer, ρ:Real) -> Integer {
  assert 0 < k;
  assert 0.0 <= ρ && ρ <= 1.0;
  cpp{{
  return std::negative_binomial_distribution<bi::type::Integer>(k, ρ)(rng);
  }}
}

/**
 * Simulate a Poisson distribution.
 *
 * - λ: Rate.
 */
function simulate_poisson(λ:Real) -> Integer {
  assert 0.0 <= λ;
  if (λ > 0.0) {
    cpp{{
    return std::poisson_distribution<bi::type::Integer>(λ)(rng);
    }}
  } else {
    return 0;
  }
}

/**
 * Simulate a categorical distribution.
 *
 * - ρ: Category probabilities. These should sum to one.
 */
function simulate_categorical(ρ:Real[_]) -> Integer {
  return simulate_categorical(ρ, 1.0);
}

/**
 * Simulate a categorical distribution.
 *
 * - ρ: Unnormalized category probabilities.
 * - Z: Sum of the unnormalized category probabilities.
 */
function simulate_categorical(ρ:Real[_], Z:Real) -> Integer {
  assert length(ρ) > 0;
  assert abs(sum(ρ) - Z) < 1.0e-6;

  u:Real <- simulate_uniform(0.0, Z);
  x:Integer <- 1;
  P:Real <- ρ[1];
  while (P < u) {
    assert x <= length(ρ);
    x <- x + 1;
    assert 0.0 <= ρ[x];
    P <- P + ρ[x];
    assert P < Z + 1.0e-6;
  }
  return x;
}

/**
 * Simulate a multinomial distribution.
 *
 * - n: Number of trials.
 * - ρ: Category probabilities. These should sum to one.
 *
 * This uses an O(N) implementation based on:
 *
 * Bentley, J. L. and J. B. Saxe (1979). Generating sorted lists of random
 * numbers. Technical Report 2450, Carnegie Mellon University, Computer
 * Science Department.
 */
function simulate_multinomial(n:Integer, ρ:Real[_]) -> Integer[_] {
  return simulate_multinomial(n, ρ, 1.0);
}

/**
 * Simulate a compound-gamma distribution.
 *
 * - k: Shape.
 * - α: Prior shape.
 * - β: Prior scale.
 *
 */
 function simulate_compound_gamma(k:Real, α:Real, β:Real) -> Real {
    return simulate_gamma(k, simulate_inverse_gamma(α, β));
 }

/**
 * Simulate a multinomial distribution.
 *
 * - n: Number of trials.
 * - ρ: Unnormalized category probabilities.
 * - Z: Sum of the unnormalized category probabilities.
 *
 * This uses an O(N) implementation based on:
 *
 * Bentley, J. L. and J. B. Saxe (1979). Generating sorted lists of random
 * numbers. Technical Report 2450, Carnegie Mellon University, Computer
 * Science Department.
 */
function simulate_multinomial(n:Integer, ρ:Real[_], Z:Real) -> Integer[_] {
  assert length(ρ) > 0;
  assert abs(sum(ρ) - Z) < 1.0e-6;

  D:Integer <- length(ρ);
  R:Real <- ρ[D];
  lnMax:Real <- 0.0;
  j:Integer <- D;
  i:Integer <- n;
  u:Real;
  x:Integer[D];
    
  while (i > 0) {
    u <- simulate_uniform(0.0, 1.0);
    lnMax <- lnMax + log(u)/i;
    u <- Z*exp(lnMax);
    while (u < Z - R) {
      j <- j - 1;
      R <- R + ρ[j];
    }
    x[j] <- x[j] + 1;
    i <- i - 1;
  }
  while (j > 1) {
    j <- j - 1;
    x[j] <- 0;
  }
  return x;
}

/**
 * Simulate a Dirichlet distribution.
 *
 * - α: Concentrations.
 */
function simulate_dirichlet(α:Real[_]) -> Real[_] {
  D:Integer <- length(α);
  x:Real[D];
  z:Real <- 0.0;

  for (i:Integer in 1..D) {
    x[i] <- simulate_gamma(α[i], 1.0);
    z <- z + x[i];
  }
  z <- 1.0/z;
  for (i:Integer in 1..D) {
    x[i] <- z*x[i];
  }
  return x;
}

/**
 * Simulate a Dirichlet distribution.
 *
 * - α: Concentration.
 * - D: Number of dimensions.
 */
function simulate_dirichlet(α:Real, D:Integer) -> Real[_] {
  assert D >= 0;
  x:Real[D];
  z:Real <- 0.0;

  for (i:Integer in 1..D) {
    x[i] <- simulate_gamma(α, 1.0);
    z <- z + x[i];
  }
  z <- 1.0/z;
  for (i:Integer in 1..D) {
    x[i] <- z*x[i];
  }
  return x;
}

/**
 * Simulate a uniform distribution.
 *
 * - l: Lower bound of interval.
 * - u: Upper bound of interval.
 */
function simulate_uniform(l:Real, u:Real) -> Real {
  assert l <= u;
  cpp{{
  return std::uniform_real_distribution<bi::type::Real>(l, u)(rng);
  }}
}

/**
 * Simulate a uniform distribution on an integer range.
 *
 * - l: Lower bound of range.
 * - u: Upper bound of range.
 */
function simulate_uniform_int(l:Integer, u:Integer) -> Integer {
  assert l <= u;
  cpp{{
  return std::uniform_int_distribution<bi::type::Integer>(l, u)(rng);
  }}
}

/**
 * Simulate a uniform distribution on unit vectors.
 *
 * - D: Number of dimensions.
 */
function simulate_uniform_unit_vector(D:Integer) -> Real[_] {
  u:Real[D];
  for d:Integer in 1..D {
    u[d] <- simulate_gaussian(0.0, 1.0);
  }
  return u/dot(u);
}

/**
 * Simulate an exponential distribution.
 *
 * - λ: Rate.
 */
function simulate_exponential(λ:Real) -> Real {
  assert 0.0 < λ;
  cpp{{
  return std::exponential_distribution<bi::type::Real>(λ)(rng);
  }}
}

/**
 * Simulate a Gaussian distribution.
 *
 * - μ: Mean.
 * - σ2: Variance.
 */
function simulate_gaussian(μ:Real, σ2:Real) -> Real {
  assert 0.0 <= σ2;
  if (σ2 == 0.0) {
    return μ;
  } else {
    cpp{{
    return std::normal_distribution<bi::type::Real>(μ, std::sqrt(σ2))(rng);
    }}
  }
}

/**
 * Simulate a log-Gaussian distribution.
 *
 * - μ: Mean (of logarithm).
 * - σ2: Variance (of logarithm).
 */
function simulate_log_gaussian(μ:Real, σ2:Real) -> Real {
  assert 0.0 <= σ2;
  if (σ2 == 0.0) {
    return μ;
  } else {
    cpp{{
    return std::lognormal_distribution<bi::type::Real>(μ, std::sqrt(σ2))(rng);
    }}
  }
}

/**
 * Simulate a Student's $t$-distribution.
 *
 * - ν: Degrees of freedom.
 */
function simulate_student_t(ν:Real) -> Real {
  assert 0.0 < ν;
  cpp{{
  return std::student_t_distribution<bi::type::Real>(ν)(rng);
  }}
}

/**
 * Simulate a Student's $t$-distribution with location and scale.
 *
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - σ2: Squared scale.
 */
function simulate_student_t(ν:Real, μ:Real, σ2:Real) -> Real {
  assert 0.0 < ν;
  assert 0.0 < σ2;
  return μ + sqrt(σ2)*simulate_student_t(ν);
}

/**
 * Simulate a beta distribution.
 *
 * - α: Shape.
 * - β: Shape.
 */
function simulate_beta(α:Real, β:Real) -> Real {
  assert 0.0 < α;
  assert 0.0 < β;
  
  u:Real <- simulate_gamma(α, 1.0);
  v:Real <- simulate_gamma(β, 1.0);
  
  return u/(u + v);
}

/**
 * Simulate a gamma distribution.
 *
 * - k: Shape.
 * - θ: Scale.
 */
function simulate_gamma(k:Real, θ:Real) -> Real {
  assert 0.0 < k;
  assert 0.0 < θ;
  cpp{{
  return std::gamma_distribution<bi::type::Real>(k, θ)(rng);
  }}
}

/**
 * Simulate an inverse-gamma distribution.
 *
 * - α: Shape.
 * - β: Scale.
 */
function simulate_inverse_gamma(α:Real, β:Real) -> Real {
  return 1.0/simulate_gamma(α, 1.0/β);
}

/**
 * Simulate a normal inverse-gamma distribution.
 *
 * - μ: Mean.
 * - a2: Variance.
 * - α: Shape of inverse-gamma on scale.
 * - β: Scale of inverse-gamma on scale.
 */
function simulate_normal_inverse_gamma(μ:Real, a2:Real, α:Real,
    β:Real) -> Real {
  return simulate_student_t(2.0*α, μ, a2*β/α);
}

/**
 * Simulate a beta-bernoulli distribution.
 *
 * - α: Shape.
 * - β: Shape.
 */
function simulate_beta_bernoulli(α:Real, β:Real) -> Boolean {
  assert 0.0 < α;
  assert 0.0 < β;
  
  return simulate_bernoulli(simulate_beta(α, β));
}

/**
 * Simulate a beta-binomial distribution.
 *
 * - n: Number of trials.
 * - α: Shape.
 * - β: Shape.
 */
function simulate_beta_binomial(n:Integer, α:Real, β:Real) -> Integer {
  assert 0 <= n;
  assert 0.0 < α;
  assert 0.0 < β;
  
  return simulate_binomial(n, simulate_beta(α, β));
}

/**
 * Simulate a beta-negative-binomial distribution.
 *
 * - k: Number of successes.
 * - α: Shape.
 * - β: Shape.
 */
function simulate_beta_negative_binomial(k:Integer, α:Real, β:Real) -> Integer {
  assert 0.0 < α;
  assert 0.0 < β;
  assert 0 < k;

  return simulate_negative_binomial(k, simulate_beta(α, β));
}


/**
 * Simulate a gamma-Poisson distribution.
 *
 * - k: Shape.
 * - θ: Scale.
 */
function simulate_gamma_poisson(k:Real, θ:Real) -> Integer {
  assert 0.0 < k;
  assert 0.0 < θ;
  assert k == floor(k);
  
  return simulate_negative_binomial(Integer(k), 1.0/(θ + 1.0));
}

/**
 * Simulate a Lomax distribution.
 *
 * - λ: Scale.
 * - α: Shape.
 */
function simulate_lomax(λ:Real, α:Real) -> Real {
  assert 0.0 < λ;
  assert 0.0 < α;

  u:Real <- simulate_uniform(0.0, 1.0);
  return λ*(pow(u, -1.0/α)-1.0);
}

/**
 * Simulate a Dirichlet-categorical distribution.
 *
 * - α: Concentrations.
 */
function simulate_dirichlet_categorical(α:Real[_]) -> Integer {
  return simulate_categorical(simulate_dirichlet(α));
}

/**
 * Simulate a Dirichlet-multinomial distribution.
 *
 * - n: Number of trials.
 * - α: Concentrations.
 */
function simulate_dirichlet_multinomial(n:Integer, α:Real[_]) ->
    Integer[_] {
  return simulate_multinomial(n, simulate_dirichlet(α));
}

/**
 * Simulate a categorical distribution with Chinese restaurant process
 * prior.
 *
 * - α: Concentration.
 * - θ: Discount.
 * - n: Enumerated items.
 * - N: Total number of items.
 */
function simulate_crp_categorical(α:Real, θ:Real, n:Integer[_],
    N:Integer) -> Integer {
  assert N >= 0;
  assert sum(n) == N;

  k:Integer <- 0;
  K:Integer <- length(n);
  if (N == 0) {
    /* first component */
    k <- 1;
  } else {
    u:Real <- simulate_uniform(0.0, N + θ);
    U:Real <- K*α + θ;
    if (u < U) {
      /* new component */
      k <- K + 1;
    } else {
      /* existing component */
      while (k < K && u > U) {
        k <- k + 1;
        U <- U + n[k] - α;
      }
    }
  }
  return k;
}

/**
 * Simulate a Gaussian distribution with an inverse-gamma prior over
 * the variance.
 *
 * - μ: Mean.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 */
function simulate_inverse_gamma_gaussian(μ:Real, α:Real, β:Real) -> Real {
  return simulate_student_t(2.0*α, μ, β/α);
}

/**
 * Simulate a Gaussian distribution with a normal inverse-gamma prior.
 *
 * - μ: Mean.
 * - a2: Variance.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 */
function simulate_normal_inverse_gamma_gaussian(μ:Real, a2:Real,
    α:Real, β:Real) -> Real {
  return simulate_student_t(2.0*α, μ, (β/α)*(1.0 + a2));
}

/**
 * Simulate a Gaussian distribution with a normal inverse-gamma prior.
 *
 * - a: Scale.
 * - μ: Mean.
 * - c: Offset.
 * - a2: Variance.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 */
function simulate_linear_normal_inverse_gamma_gaussian(a:Real, μ:Real,
    c:Real, a2:Real, α:Real, β:Real) -> Real {
  return simulate_student_t(2.0*α, a*μ + c, (β/α)*(1.0 + a*a*a2));
}

/**
 * Simulate a multivariate Gaussian distribution.
 *
 * - μ: Mean.
 * - Σ: Covariance.
 */
function simulate_multivariate_gaussian(μ:Real[_], Σ:Real[_,_]) -> Real[_] {
  D:Integer <- length(μ);
  z:Real[D];
  for (d:Integer in 1..D) {
    z[d] <- simulate_gaussian(0.0, 1.0);
  }
  return μ + chol(Σ)*z;
}

/**
 * Simulate a multivariate Gaussian distribution with diagonal covariance.
 *
 * - μ: Mean.
 * - σ2: Variance.
 */
function simulate_multivariate_gaussian(μ:Real[_], σ2:Real) -> Real[_] {
  D:Integer <- length(μ);
  z:Real[D];
  σ:Real <- sqrt(σ2);
  for (d:Integer in 1..D) {
    z[d] <- μ[d] + σ*simulate_gaussian(0.0, 1.0);
  }
  return z;
}

/**
 * Simulate a multivariate Student's $t$-distribution variate with
 * location and scale.
 *
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - Λ: Precision.
 */
function simulate_multivariate_student_t(ν:Real, μ:Real[_], Λ:Real[_,_]) ->
    Real[_] {
  D:Integer <- length(μ);
  z:Real[D];
  for (d:Integer in 1..D) {
    z[d] <- simulate_student_t(ν);
  }
  return μ + solve(trans(chol(Λ)), z);
}

/**
 * Simulate a multivariate Student's $t$-distribution variate with
 * location and diagonal scaling.
 *
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - λ: Precision.
 */
function simulate_multivariate_student_t(ν:Real, μ:Real[_], λ:Real) ->
    Real[_] {
  D:Integer <- length(μ);
  z:Real[D];
  σ:Real <- sqrt(1.0/λ);
  for (d:Integer in 1..D) {
    z[d] <- μ[d] + σ*simulate_student_t(ν);
  }
  return z;
}

/**
 * Simulate a multivariate normal inverse-gamma distribution.
 *
 * - μ: Mean.
 * - Λ: Precision.
 * - α: Shape of inverse-gamma on scale.
 * - β: Scale of inverse-gamma on scale.
 */
function simulate_multivariate_normal_inverse_gamma(μ:Real[_], Λ:Real[_,_],
    α:Real, β:Real) -> Real[_] {
  return simulate_multivariate_student_t(2.0*α, μ, Λ*(α/β));
}

/**
 * Simulate a multivariate Gaussian distribution with an inverse-gamma prior
 * over a diagonal covariance.
 *
 * - μ: Mean.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 */
function simulate_multivariate_inverse_gamma_gaussian(μ:Real[_], α:Real,
    β:Real) -> Real[_] {
  D:Integer <- length(μ);
  z:Real[D];
  a:Real <- sqrt(β/α);
  for (d:Integer in 1..D) {
    z[d] <- μ[d] + a*simulate_student_t(2.0*α);
  }
  return z;
}

/**
 * Simulate a multivariate Gaussian distribution with a multivariate normal
 * inverse-gamma prior.
 *
 * - μ: Mean.
 * - Λ: Precision.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 */
function simulate_multivariate_normal_inverse_gamma_gaussian(μ:Real[_],
    Λ:Real[_,_], α:Real, β:Real) -> Real[_] {
  return simulate_multivariate_student_t(2.0*α, μ,
      (α/β)*cholinv(identity(rows(Λ)) + cholinv(Λ)));
}

/**
 * Simulate a Gaussian distribution with a multivariate linear normal
 * inverse-gamma prior.
 *
 * - A: Scale.
 * - μ: Mean.
 * - c: Offset.
 * - Λ: Precision.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 */
function simulate_multivariate_linear_normal_inverse_gamma_gaussian(
    A:Real[_,_], μ:Real[_], c:Real[_], Λ:Real[_,_], α:Real, β:Real) -> Real[_] {
  return simulate_multivariate_student_t(2.0*α, A*μ + c,
      (α/β)*cholinv(identity(rows(A)) + A*cholsolve(Λ, trans(A))));
}

/**
 * Simulate a Gaussian distribution with a multivariate dot normal inverse-gamma
 * prior.
 *
 * - a: Scale.
 * - μ: Mean.
 * - c: Offset.
 * - Λ: Precision.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 */
function simulate_multivariate_dot_normal_inverse_gamma_gaussian(
    a:Real[_], μ:Real[_], c:Real, Λ:Real[_,_], α:Real, β:Real) -> Real {
  return simulate_student_t(2.0*α, dot(a, μ) + c,
      (β/α)*(1.0 + dot(a, cholsolve(Λ, a))));
}

/**
 * Simulate a multivariate uniform distribution.
 *
 * - l: Lower bound of hyperrectangle.
 * - u: Upper bound of hyperrectangle.
 */
function simulate_multivariate_uniform(l:Real[_], u:Real[_]) -> Real[_] {
  assert length(l) == length(u);
  D:Integer <- length(l);
  z:Real[D];
  for (d:Integer in 1..D) {
    z[d] <- simulate_uniform(l[d], u[d]);
  }
  return z;
}

/**
 * Simulate a multivariate uniform distribution over integers.
 *
 * - l: Lower bound of hyperrectangle.
 * - u: Upper bound of hyperrectangle.
 */
function simulate_multivariate_uniform_int(l:Integer[_], u:Integer[_]) -> Integer[_] {
  assert length(l) == length(u);
  D:Integer <- length(l);
  z:Integer[D];
  for (d:Integer in 1..D) {
    z[d] <- simulate_uniform_int(l[d], u[d]);
  }
  return z;
}
