/*
 * Delayed Dirichlet random variate.
 */
final class DelayDirichlet(future:Real[_]?, futureUpdate:Boolean, α:Real[_])
    < DelayValue<Real[_]>(future, futureUpdate) {
  /**
   * Concentrations.
   */
  α:Real[_] <- α;

  function simulate() -> Real[_] {
    return simulate_dirichlet(α);
  }
  
  function observe(x:Real[_]) -> Real {
    return observe_dirichlet(x, α);
  }

  function update(x:Real[_]) {
    //
  }

  function downdate(x:Real[_]) {
    //
  }

  function pdf(x:Real[_]) -> Real {
    return pdf_dirichlet(x, α);
  }

  function write(buffer:Buffer) {
    prune();
    buffer.set("class", "Dirichlet");
    buffer.set("α", α);
  }
}

function DelayDirichlet(future:Real[_]?, futureUpdate:Boolean, α:Real[_]) ->
    DelayDirichlet {
  m:DelayDirichlet(future, futureUpdate, α);
  return m;
}
