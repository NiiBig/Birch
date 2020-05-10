/**
 * (Metropolis-adjusted) Langevin Markov kernel.
 *
 * The Kernel class hierarchy is as follows:
 * <center>
 * <object type="image/svg+xml" data="../../figs/Kernel.svg"></object>
 * </center>
 */
class LangevinKernel < Kernel {
  /**
   * Scale of the move.
   */
  scale:Real <- 1.0;

  override function move(x:Random<Real>) -> Real {
    return simulate_gaussian(x.x! + scale*x.dfdx!, 2.0*scale);
  }

  override function move(x:Random<Real[_]>) -> Real[_] {
    return simulate_multivariate_gaussian(x.x! + scale*x.dfdx!, 2.0*scale);
  }

  override function move(x:Random<Real[_,_]>) -> Real[_,_] {
    return simulate_matrix_gaussian(x.x! + scale*x.dfdx!, 2.0*scale);
  }

  override function logpdf(x':Random<Real>, x:Random<Real>) -> Real {
    return logpdf_gaussian(x'.x!, x.x! + scale*x.dfdx!, 2.0*scale);
  }

  override function logpdf(x':Random<Real[_]>, x:Random<Real[_]>) -> Real {
    return logpdf_multivariate_gaussian(x'.x!, x.x! + scale*x.dfdx!, 2.0*scale);
  }

  override function logpdf(x':Random<Real[_,_]>, x:Random<Real[_,_]>) -> Real {
    return logpdf_matrix_gaussian(x'.x!, x.x! + scale*x.dfdx!, 2.0*scale);
  }

  /**
   * Finalize contribution to the log-acceptance probability for the
   * proposed and current states.
   *
   * - x': Proposed state $x^\prime$.
   * - x: Current state $x$.
   *
   * Returns: Log-ratio of proposal densities, $\log q(x \mid x^\prime) -
   * \log q(x^\prime \mid x)$.
   */
  override function zip(x':Random<Real>, x:Random<Real>) -> Real {
    return logpdf(x, x') - logpdf(x', x);
  }

  /**
   * Finalize contribution to the log-acceptance probability for the
   * proposed and current states.
   *
   * - x': Proposed state $x^\prime$.
   * - x: Current state $x$.
   *
   * Returns: Log-ratio of proposal densities, $\log q(x \mid x^\prime) -
   * \log q(x^\prime \mid x)$.
   */
  override function zip(x':Random<Real[_]>, x:Random<Real[_]>) -> Real {
    return logpdf(x, x') - logpdf(x', x);
  }

  /**
   * Finalize contribution to the log-acceptance probability for the
   * proposed and current states.
   *
   * - x': Proposed state $x^\prime$.
   * - x: Current state $x$.
   *
   * Returns: Log-ratio of proposal densities, $\log q(x \mid x^\prime) -
   * \log q(x^\prime \mid x)$.
   */
  override function zip(x':Random<Real[_,_]>, x:Random<Real[_,_]>) -> Real {
    return logpdf(x, x') - logpdf(x', x);
  }
  
  override function read(buffer:Buffer) {
    super.read(buffer);
    scale <-? buffer.getReal("scale");
  }
  
  override function write(buffer:Buffer) {
    super.write(buffer);
    buffer.setReal("scale", scale);
  }
}
