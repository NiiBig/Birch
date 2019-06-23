/**
 * Uniform distribution.
 */
final class Uniform(l:Expression<Real>, u:Expression<Real>) < Distribution<Real> {
  /**
   * Lower bound.
   */
  l:Expression<Real> <- l;
  
  /**
   * Upper bound.
   */
  u:Expression<Real> <- u;

  function simulateForward() -> Real {
    assert !delay?;
    return simulate_uniform(l, u);
  }

  function logpdfForward(x:Real) -> Real {
    assert !delay?;
    return logpdf_uniform(x, l, u);
  }

  function graft() {
    if delay? {
      delay!.prune();
    } else {
      delay <- DelayUniform(future, futureUpdate, l, u);
    }
  }

  function write(buffer:Buffer) {
    if delay? {
      delay!.write(buffer);
    } else {
      buffer.set("class", "Uniform");
      buffer.set("l", l.value());
      buffer.set("u", u.value());
    }
  }
}

/**
 * Create a uniform distribution.
 */
function Uniform(l:Expression<Real>, u:Expression<Real>) -> Uniform {
  m:Uniform(l, u);
  return m;
}

/**
 * Create a uniform distribution.
 */
function Uniform(l:Expression<Real>, u:Real) -> Uniform {
  return Uniform(l, Boxed(u));
}

/**
 * Create a uniform distribution.
 */
function Uniform(l:Real, u:Expression<Real>) -> Uniform {
  return Uniform(Boxed(l), u);
}

/**
 * Create a uniform distribution.
 */
function Uniform(l:Real, u:Real) -> Uniform {
  return Uniform(Boxed(l), Boxed(u));
}
