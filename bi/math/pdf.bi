cpp{{
#include "boost/math/distributions.hpp"
}}

/**
 * PMF of a Bernoulli variate.
 *
 * - x: The variate.
 * - ρ: Probability of a true result.
 *
 * Returns: the probability mass.
 */
function pmf_bernoulli(x:Boolean, ρ:Real) -> Real {
  assert 0.0 <= ρ && ρ <= 1.0;
  if (x) {
    return ρ;
  } else {
    return 1.0 - ρ;
  }
}

/**
 * PMF of a delta variate.
 *
 * - x: The variate.
 * - μ: Location.
 *
 * Returns: the probability mass.
 */
function pmf_delta(x:Integer, μ:Integer) -> Real {
  if (x == μ) {
    return 1.0;
  } else {
    return 0.0;
  }
}

/**
 * PMF of a binomial variate.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - ρ: Probability of a true result.
 *
 * Returns: the probability mass.
 */
function pmf_binomial(x:Integer, n:Integer, ρ:Real) -> Real {
  assert 0 <= n;
  assert 0.0 <= ρ && ρ <= 1.0;
  cpp{{
  return boost::math::pdf(boost::math::binomial_distribution<>(n_, ρ_), x_);
  }}
}

/**
 * PMF of a negative binomial variate.
 *
 * - x: The variate (number of failures).
 * - k: Number of successes before the experiment is stopped.
 * - ρ: Probability of success.
 *
 * Returns: the probability mass.
 */
function pmf_negative_binomial(x:Integer, k:Integer, ρ:Real) -> Real {
  assert 0 < k;
  assert 0.0 <= ρ && ρ <= 1.0;
  cpp{{
  return boost::math::pdf(boost::math::negative_binomial_distribution<>(k_, ρ_), x_);
  }}
}

/**
 * PMF of a Poisson variate.
 *
 * - x: The variate.
 * - λ: Rate.
 *
 * Returns: the probability mass.
 */
function pmf_poisson(x:Integer, λ:Real) -> Real {
  assert 0.0 <= λ;
  cpp{{
  return boost::math::pdf(boost::math::poisson_distribution<>(λ_), x_);
  }}
}

/**
 * PMF of an integer uniform variate.
 *
 * - x: The variate.
 * - l: Lower bound of interval.
 * - u: Upper bound of interval.
 *
 * Returns: the probability mass.
 */
function pmf_int_uniform(x:Integer, l:Integer, u:Integer) -> Real {
  if (x >= l && x <= u) {
    return 1.0/(u - l + 1);
  } else {
    return 0.0;
  }
}

/**
 * PMF of a categorical variate.
 *
 * - x: The variate.
 * - ρ: Category probabilities.
 *
 * Returns: the probability mass.
 */
function pmf_categorical(x:Integer, ρ:Real[_]) -> Real {
  if (1 <= x && x <= length(ρ)) {
    assert ρ[x] >= 0.0;
    return ρ[x];
  } else {
    return 0.0;
  }
}

/**
 * PMF of a multinomial variate.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - ρ: Category probabilities.
 *
 * Returns: the probability mass.
 */
function pmf_multinomial(x:Integer[_], n:Integer, ρ:Real[_]) -> Real {
  return exp(observe_multinomial(x, n, ρ));
}

/**
 * PMF of a Dirichlet variate.
 *
 * - x: The variate.
 * - α: Concentrations.
 *
 * Returns: the probability density.
 */
function pmf_dirichlet(x:Real[_], α:Real[_]) -> Real {
  return exp(observe_dirichlet(x, α));
}

/**
 * PDF of a uniform variate.
 *
 * - x: The variate.
 * - l: Lower bound of interval.
 * - u: Upper bound of interval.
 *
 * Returns: the probability density.
 */
function pdf_uniform(x:Real, l:Real, u:Real) -> Real {
  assert l <= u;

  if (x >= l && x <= u) {
    return 1.0/(u - l);
  } else {
    return 0.0;
  }
}

/**
 * PDF of an exponential variate.
 *
 * - x: The variate.
 * - λ: Rate.
 *
 * Returns: the probability density.
 */
function pdf_exponential(x:Real, λ:Real) -> Real {
  assert 0.0 < λ;
  cpp{{
  return boost::math::pdf(boost::math::exponential_distribution<>(λ_), x_);
  }}
}

/**
 * PDF of a Gaussian variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - σ2: Variance.
 *
 * Returns: the probability density.
 */
function pdf_gaussian(x:Real, μ:Real, σ2:Real) -> Real {
  assert 0.0 < σ2;
  cpp{{
  return boost::math::pdf(boost::math::normal_distribution<>(μ_, ::sqrt(σ2_)), x_);
  }}
}

/**
 * PDF of a log-Gaussian variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - σ2: Variance.
 *
 * Returns: the probability density.
 */
function pdf_log_gaussian(x:Real, μ:Real, σ2:Real) -> Real {
  assert 0.0 < σ2;
  cpp{{
  return boost::math::pdf(boost::math::lognormal_distribution<>(μ_, ::sqrt(σ2_)), x_);
  }}
}

/**
 * PDF of a Student's $t$ variate.
 *
 * - x: The variate.
 * - ν: Degrees of freedom.
 *
 * Returns: the probability density.
 */
function pdf_student_t(x:Real, ν:Real) -> Real {
  assert 0.0 < ν;
  cpp{{
  return boost::math::pdf(boost::math::students_t_distribution<>(ν_), x_);
  }}
}

/**
 * PDF of a Student's $t$ variate with location and scale.
 *
 * - x: The variate.
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - σ2: Squared scale.
 *
 * Returns: the probability density.
 */
function pdf_student_t(x:Real, ν:Real, μ:Real, σ2:Real) -> Real {
  assert 0.0 < ν;
  assert 0.0 < σ2;
  σ:Real <- sqrt(σ2);
  return pdf_student_t((x - μ)/σ, ν)/σ;
}

/**
 * PDF of a beta variate.
 *
 * - x: The variate.
 * - α: Shape.
 * - β: Shape.
 *
 * Returns: the probability density.
 */
function pdf_beta(x:Real, α:Real, β:Real) -> Real {
  assert 0.0 < α;
  assert 0.0 < β;
  cpp{{
  return boost::math::pdf(boost::math::beta_distribution<>(α_, β_), x_);
  }}
}

/**
 * PDF of a gamma variate.
 *
 * - x: The variate.
 * - k: Shape.
 * - θ: Scale.
 *
 * Returns: the probability density.
 */
function pdf_gamma(x:Real, k:Real, θ:Real) -> Real {
  assert 0.0 < k;
  assert 0.0 < θ;
  cpp{{
  return boost::math::pdf(boost::math::gamma_distribution<>(k_, θ_), x_);
  }}
}

/**
 * PDF of an inverse-gamma variate.
 *
 * - x: The variate.
 * - α: Shape.
 * - β: Scale.
 *
 * Returns: the probability density.
 */
function pdf_inverse_gamma(x:Real, α:Real, β:Real) -> Real {
  assert 0.0 < α;
  assert 0.0 < β;
  cpp{{
  return boost::math::pdf(boost::math::inverse_gamma_distribution<>(α_, β_), x_);
  }}
}

/**
 * PDF of a normal inverse-gamma variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - a2: Variance.
 * - α: Shape of inverse-gamma on scale.
 * - β: Scale of inverse-gamma on scale.
 *
 * Returns: the probability density.
 */
function pdf_normal_inverse_gamma(x:Real, μ:Real, a2:Real, α:Real,
    β:Real) -> Real {
  return pdf_student_t(x, 2.0*α, μ, a2*β/α);
}

/**
 * PMF of a beta-binomial variate.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - α: Shape.
 * - β: Shape.
 *
 * Returns: the probability mass.
 */
function pmf_beta_binomial(x:Integer, n:Integer, α:Real, β:Real) -> Real {
  return exp(observe_beta_binomial(x, n, α, β));
}

/**
 * PMF of a gamma-Poisson variate.
 *
 * - x: The variate.
 * - k: Shape.
 * - θ: Scale.
 *
 * Returns: the probability mass.
 */
function pmf_gamma_poisson(x:Integer, k:Integer, θ:Real) -> Real {
  assert 0.0 < k;
  assert 0.0 < θ;
  return pmf_negative_binomial(x, k, 1.0/(θ + 1.0));
}

/**
 * PMF of a Dirichlet-categorical variate.
 *
 * - x: The variate.
 * - α: Concentrations.
 *
 * Returns: the probability mass.
 */
function pmf_dirichlet_categorical(x:Integer, α:Real[_]) -> Real {
  return exp(observe_dirichlet_categorical(x, α));
}

/**
 * PMF of a Dirichlet-multinomial variate.
 *
 * - x: The variate.
 * - n: Number of trials.
 * - α: Concentrations.
 *
 * Returns: the probability mass.
 */
function pmf_dirichlet_multinomial(x:Integer[_], n:Integer, α:Real[_]) -> Real {
  return exp(observe_dirichlet_multinomial(x, n, α));
}

/**
 * PMF of a categorical variate with Chinese restaurant process prior.
 *
 * - x: The variate.
 * - α: Concentration.
 * - θ: Discount.
 * - n: Enumerated items.
 * - N: Total number of items.
 */
function pmf_crp_categorical(k:Integer, α:Real, θ:Real,
    n:Integer[_], N:Integer) -> Real {
  K:Integer <- length(n);
  if (k > K + 1) {
    return 0.0;
  } else if (k == K + 1) {
    return (K*α + θ)/(N + θ);
  } else {
    return (n[k] - α)/(N + θ);
  }
}

/**
 * PDF of a Gaussian variate with an inverse-gamma distribution over
 * the variance.
 *
 * - x: The variate.
 * - μ: Mean.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the probability density.
 */
function pdf_inverse_gamma_gaussian(x:Real, μ:Real, α:Real,
    β:Real) -> Real {
  return pdf_student_t(x, 2.0*α, μ, β/α);
}

/**
 * PDF of a Gaussian variate with a normal inverse-gamma prior.
 *
 * - x: The variate.
 * - μ: Mean.
 * - a2: Variance.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the probability density.
 */
function pdf_normal_inverse_gamma_gaussian(x:Real, μ:Real, a2:Real,
    α:Real, β:Real) -> Real {
  return pdf_student_t(x, 2.0*α, μ, (β/α)*(1.0 + a2));
}

/**
 * PDF of a Gaussian variate with a normal inverse-gamma prior with affine
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
 * Returns: the probability density.
 */
function pdf_affine_normal_inverse_gamma_gaussian(x:Real, a:Real,
    μ:Real, c:Real, a2:Real, α:Real, β:Real) -> Real {
  return pdf_student_t(x, 2.0*α, a*μ + c, (β/α)*(1.0 + a*a*a2));
}

/**
 * PDF of a multivariate Gaussian variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - Σ: Covariance.
 *
 * Returns: the probability density.
 */
function pdf_multivariate_gaussian(x:Real[_], μ:Real[_], Σ:Real[_,_]) ->
    Real {
  return exp(observe_multivariate_gaussian(x, μ, Σ));
}

/**
 * PDF of a multivariate Gaussian distribution with diagonal covariance.
 *
 * - x: The variate.
 * - μ: Mean.
 * - σ2: Variance.
 *
 * Returns: the probability density.
 */
function pdf_multivariate_gaussian(x:Real[_], μ:Real[_], σ2:Real) -> Real {
  return exp(observe_multivariate_gaussian(x, μ, σ2));
}

/**
 * PDF of a multivariate Student's $t$-distribution variate with location
 * and scale.
 *
 * - x: The variate.
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - Λ: Precision.
 *
 * Returns: the probability density.
 */
function pdf_multivariate_student_t(x:Real[_], ν:Real, μ:Real[_],
    Λ:Real[_,_]) -> Real {
  return exp(observe_multivariate_student_t(x, ν, μ, Λ));
}

/**
 * PDF of a multivariate Student's $t$-distribution variate with location
 * and diagonal scale.
 *
 * - x: The variate.
 * - ν: Degrees of freedom.
 * - μ: Location.
 * - λ: Precision.
 *
 * Returns: the probability density.
 */
function pdf_multivariate_student_t(x:Real[_], ν:Real, μ:Real[_],
    λ:Real) -> Real {
  return exp(observe_multivariate_student_t(x, ν, μ, λ));
}

/**
 * PDF of a multivariate normal inverse-gamma variate.
 *
 * - x: The variate.
 * - μ: Mean.
 * - Λ: Precision.
 * - α: Shape of inverse-gamma on scale.
 * - β: Scale of inverse-gamma on scale.
 *
 * Returns: the probability density.
 */
function pdf_multivariate_normal_inverse_gamma(x:Real[_], μ:Real[_],
    Λ:Real[_,_], α:Real, β:Real) -> Real {
  return pdf_multivariate_student_t(x, 2.0*α, μ, Λ*(α/β));
}

/**
 * PDF of a multivariate Gaussian variate with an inverse-gamma distribution
 * over a diagonal covariance.
 *
 * - x: The variate.
 * - μ: Mean.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the probability density.
 */
function pdf_multivariate_inverse_gamma_gaussian(x:Real[_], μ:Real[_],
    α:Real, β:Real) -> Real {
  return pdf_multivariate_student_t(x, 2.0*α, μ, β/α);
}

/**
 * PDF of a multivariate Gaussian variate with a multivariate normal
 * inverse-gamma prior.
 *
 * - x: The variate.
 * - μ: Mean.
 * - Λ: Precision.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the probability density.
 */
function pdf_multivariate_normal_inverse_gamma_gaussian(x:Real[_], μ:Real[_],
    Λ:Real[_,_], α:Real, β:Real) -> Real {
  return exp(observe_multivariate_normal_inverse_gamma_gaussian(x, μ, Λ, α, β));
}

/**
 * PDF of a multivariate Gaussian variate with a multivariate normal
 * inverse-gamma prior with affine transformation.
 *
 * - x: The variate.
 * - A: Scale.
 * - μ: Mean.
 * - c: Offset.
 * - Λ: Precision.
 * - α: Shape of the inverse-gamma.
 * - β: Scale of the inverse-gamma.
 *
 * Returns: the probability density.
 */
function pdf_multivariate_affine_normal_inverse_gamma_gaussian(x:Real[_],
    A:Real[_,_], μ:Real[_], c:Real[_], Λ:Real[_,_], α:Real, β:Real) -> Real {
  return exp(observe_multivariate_affine_normal_inverse_gamma_gaussian(x, A,
      μ, c, Λ, α, β));
}
