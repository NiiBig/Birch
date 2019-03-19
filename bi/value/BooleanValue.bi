/**
 * Boolean value.
 */
class BooleanValue(value:Boolean) < Value {
  /**
   * The value.
   */
  value:Boolean <- value;
  
  operator -> Boolean {
    return value;
  }

  function accept(gen:Generator) {
    gen.visit(this);
  }

  function isValue() -> Boolean {
    return true;
  }
  
  function getBoolean() -> Boolean? {
    return value;
  }
}
