/**
 * Random access model.
 */
class RandomAccessModel < BidirectionalModel {
  /**
   * Current step.
   */
  t:Integer <- 0;

  /**
   * Seek to a specific step.
   */
  function seek(t:Integer) {
    this.t <- t;
  }
  
  function skip() {
    seek(t + 1);
  }
  
  function back() {
    seek(t - 1);
  }
}
