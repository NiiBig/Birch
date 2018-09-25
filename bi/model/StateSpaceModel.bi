/**
 * State-space model.
 *
 * - SpecificVariate: Specialization of StateSpaceVariate.
 *
 * The joint distribution is:
 *
 * $$p(\mathrm{d}\theta, \mathrm{d}x_{0:T}, \mathrm{d}y_{0:T}) =
 *   p(\mathrm{d}\theta) p(\mathrm{d}x_0 \mid \theta)  p(\mathrm{d}y_0
 *   \mid x_0, \theta) \prod_{t=1}^T p(\mathrm{d}x_t \mid x_{t-1},
 *   \theta) p(\mathrm{d}y_t \mid x_t, \theta)$$
 */
class StateSpaceModel<SpecificVariate> < ModelFor<SpecificVariate> {
  fiber simulate(v:SpecificVariate) -> Real {
    /* parameter */
    auto θ <- v.θ;
    yield sum(p(θ));

    auto xs <- v.x.walk();
    auto ys <- v.y.walk();    
    
    /* initial state and initial observation */
    x:SpecificVariate.State;
    y:SpecificVariate.Observation;
    
    if (xs?) {
      x <- xs!;
    }
    if (ys?) {
      y <- ys!;
    }
    yield sum(m(x, θ)) + sum(g(y, x, θ));
    
    /* transition and observation */
    while (true) {
      x0:SpecificVariate.State <- x;
      if (xs?) {
        x <- xs!;
      } else {
        x':SpecificVariate.State;
        x <- x';
        v.x.pushBack(x);
      }
      if (ys?) {
        y <- ys!;
      } else {
        y':SpecificVariate.Observation;
        y <- y';
        v.y.pushBack(y);
      }
      yield sum(f(x, x0, θ)) + sum(g(y, x, θ));
    }
  }
  
  /**
   * Parameter model.
   */
  fiber p(θ':SpecificVariate.Parameter) -> Real {
    //
  }
  
  /**
   * Initial model.
   */
  fiber m(x':SpecificVariate.State, θ:SpecificVariate.Parameter) -> Real {
    //
  }
  
  /**
   * Transition model.
   */
  fiber f(x':SpecificVariate.State, x:SpecificVariate.State,
      θ:SpecificVariate.Parameter) -> Real {
    //
  }
  
  /**
   * Observation model.
   */
  fiber g(x':SpecificVariate.Observation, x:SpecificVariate.State,
      θ:SpecificVariate.Parameter) -> Real {
    //
  }
}
