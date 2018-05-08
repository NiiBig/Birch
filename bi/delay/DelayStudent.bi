/*
 * Delayed Student's $t$ random variate.
 */
class DelayStudent(ν:Real) < DelayValue<Real> {
  /**
   * Degrees of freedom.
   */
  ν:Real <- ν;
  
  function simulate() -> Real {
    return simulate_student_t(ν);
  }
  
  function observe(x:Real) -> Real {
    return observe_student_t(x, ν);
  }

  function pdf(x:Real) -> Real {
    return pdf_student_t(x, ν);
  }

  function cdf(x:Real) -> Real {
    return cdf_student_t(x, ν);
  }
}

function DelayStudent(ν:Real) -> DelayStudent {
  m:DelayStudent(ν);
  return m;
}
