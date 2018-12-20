/**
 * Abstract buffer.
 *
 * When reading and writing objects, defers to the `read()` and `write()`
 * member functions, declared in `Object`.
 */
class Buffer {
  /**
   * Get this.
   */
  function get() -> Buffer;

  /**
   * Get this as an object.
   */
  function getObject() -> Buffer? {
    return get().getObject();
  }

  /**
   * Get this as an array.
   *
   * Return: a fiber object that yields each of the elements of the array in
   * turn, or which never yields if this is an empty array or not an array
   * at all.
   */
  fiber getArray() -> Buffer {
    get().getArray();
  }
  
  /**
   * If this is an array get its length.
   *
   * Return: An optional with a value giving the length if this is an array.
   */
  function getLength() -> Integer? {
    return get().getLength();
  }

  /**
   * Get this as a Boolean.
   *
   * Return: An optional with a value if this is of a compatible type.
   */
  function getBoolean() -> Boolean? {
    return get().getBoolean();
  }

  /**
   * Get this as an integer.
   *
   * Return: An optional with a value if this is of a compatible type.
   */
  function getInteger() -> Integer? {
    return get().getInteger();
  }

  /**
   * Get this as a real.
   *
   * Return: An optional with a value if this is of a compatible type.
   */
  function getReal() -> Real? {
    return get().getReal();
  }

  /**
   * Get this as a string.
   *
   * Return: An optional with a value if this is of a compatible type.
   */
  function getString() -> String? {
    return get().getString();
  }
  
  /**
   * Get this as an object.
   *
   * - value: The object into which to read.
   *
   * Return: The object.
   */
  function getObject(value:Object) -> Object? {
    return get().getObject(value);
  }

  /**
   * Get this as a vector of Booleans.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function getBooleanVector() -> Boolean[_]? {
    return get().getBooleanVector();
  }

  /**
   * Get this as a vector of integers.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function getIntegerVector() -> Integer[_]? {
    return get().getIntegerVector();
  }

  /**
   * Get this as a vector of reals.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function getRealVector() -> Real[_]? {
    return get().getRealVector();
  }

  /**
   * Get this as a matrix of Booleans.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function getBooleanMatrix() -> Boolean[_,_]? {
    return get().getBooleanMatrix();
  }

  /**
   * Get this as a matrix of integers.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function getIntegerMatrix() -> Integer[_,_]? {
    return get().getIntegerMatrix();
  }

  /**
   * Get this as a matrix of reals.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function getRealMatrix() -> Real[_,_]? {
    return get().getRealMatrix();
  }

  /**
   * Get a child.
   */
  function getChild(name:String) -> Buffer?;

  /**
   * Get a child as an object.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function getObject(name:String) -> Buffer? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getObject();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as an array.
   *
   * - name: Name of the child.
   *
   * Return: a fiber object that yields each of the elements of the array in
   * turn, or which never yields if this is an empty array or not an array
   * at all.
   */
  fiber getArray(name:String) -> Buffer {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      buffer!.getArray();
    }
  }

  /**
   * Get the length of an array.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value giving the length if the given entry
   * is an array.
   */
  function getLength(name:String) -> Integer? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getLength();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as a Boolean.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function getBoolean(name:String) -> Boolean? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getBoolean();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as an integer.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function getInteger(name:String) -> Integer? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getInteger();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as a real.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function getReal(name:String) -> Real? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getReal();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as a string.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function getString(name:String) -> String? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getString();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as an object.
   *
   * - name: Name of the child.
   * - value: The object into which to read.
   *
   * Return: An optional with a value if the given entry exists.
   */
  function getObject(name:String, value:Object) -> Object? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getObject(value);
    } else {
      return nil;
    }
  }


  /**
   * Get a child as a vector of Booleans.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function getBooleanVector(name:String) -> Boolean[_]? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getBooleanVector();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as a vector of integers.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function getIntegerVector(name:String) -> Integer[_]? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getIntegerVector();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as a vector of reals.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function getRealVector(name:String) -> Real[_]? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getRealVector();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as a matrix of Booleans.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function getBooleanMatrix(name:String) -> Boolean[_,_]? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getBooleanMatrix();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as a matrix of integers.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function getIntegerMatrix(name:String) -> Integer[_,_]? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getIntegerMatrix();
    } else {
      return nil;
    }
  }

  /**
   * Get a child as a matrix of reals.
   *
   * - name: Name of the child.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function getRealMatrix(name:String) -> Real[_,_]? {
    buffer:Buffer? <- getChild(name);
    if (buffer?) {
      return buffer!.getRealMatrix();
    } else {
      return nil;
    }
  }

  /**
   * Get this as a Boolean.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is of a compatible type.
   */
  function get(value:Boolean?) -> Boolean? {
    return getBoolean();
  }

  /**
   * Get this as an integer.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is of a compatible type.
   */
  function get(value:Integer?) -> Integer? {
    return getInteger();
  }
  
  /**
   * Get this as a real.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is of a compatible type.
   */
  function get(value:Real?) -> Real? {
    return getReal();
  }

  /**
   * Get this as a string.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is of a compatible type.
   */
  function get(value:String?) -> String? {
    return getString();
  }
  
  /**
   * Get this as an object.
   *
   * - value: The object.
   *
   * Return: The object.
   */
  function get(value:Object) -> Object? {
    return getObject(value);
  }

  /**
   * Get this as a vector of Booleans.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function get(value:Boolean[_]?) -> Boolean[_]? {
    return getBooleanVector();
  }

  /**
   * Get this as a vector of integers.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function get(value:Integer[_]?) -> Integer[_]? {
    return getIntegerVector();
  }

  /**
   * Get this as a vector of reals.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function get(value:Real[_]?) -> Real[_]? {
    return getRealVector();
  }

  /**
   * Get this as a matrix of Booleans.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function get(value:Boolean[_,_]?) -> Boolean[_,_]? {
    return getBooleanMatrix();
  }

  /**
   * Get this as a matrix of integers.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function get(value:Integer[_,_]?) -> Integer[_,_]? {
    return getIntegerMatrix();
  }

  /**
   * Get this as a matrix of reals.
   *
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function get(value:Real[_,_]?) -> Real[_,_]? {
    return getRealMatrix();
  }

  /**
   * Get a Boolean.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function get(name:String, value:Boolean?) -> Boolean? {
    return getBoolean(name);
  }

  /**
   * Get an integer.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function get(name:String, value:Integer?) -> Integer? {
    return getInteger(name);
  }

  /**
   * Get a real.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function get(name:String, value:Real?) -> Real? {
    return getReal(name);
  }

  /**
   * Get a string.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if the given entry exists and is of a
   * compatible type.
   */
  function get(name:String, value:String?) -> String? {
    return getString(name);
  }

  /**
   * Get an object.
   *
   * - name: Name of the child.
   * - value: The object.
   *
   * Return: The object.
   */
  function get(name:String, value:Object) -> Object? {
    return getObject(name, value);
  }

  /**
   * Get a vector of Booleans.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function get(name:String, value:Boolean[_]?) -> Boolean[_]? {
    return getBooleanVector(name);
  }

  /**
   * Get a vector of integers.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function get(name:String, value:Integer[_]?) -> Integer[_]? {
    return getIntegerVector(name);
  }

  /**
   * Get a vector of reals.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array with all elements
   * of a compatible type.
   */
  function get(name:String, value:Real[_]?) -> Real[_]? {
    return getRealVector(name);
  }

  /**
   * Get a matrix of Booleans.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function get(name:String, value:Boolean[_,_]?) -> Boolean[_,_]? {
    return getBooleanMatrix(name);
  }

  /**
   * Get a matrix of integers.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function get(name:String, value:Integer[_,_]?) -> Integer[_,_]? {
    return getIntegerMatrix(name);
  }

  /**
   * Get a matrix of reals.
   *
   * - name: Name of the child.
   * - value: Unused, but necessary for overload resolution.
   *
   * Return: An optional with a value if this is an array where all elements
   * are themselves arrays of the same length and compatible type.
   */
  function get(name:String, value:Real[_,_]?) -> Real[_,_]? {
    return getRealMatrix(name);
  }
  
  /**
   * Set this (to nil).
   */
  function set() -> Buffer;
  
  /**
   * Set this as an object.
   */
  function setObject() {
    set().setObject();
  }
  
  /**
   * Set this as an array.
   */
  function setArray() {
    set().setArray();
  }

  /**
   * Set this as nil.
   */
  function setNil() {
    set().setNil();
  }

  /**
   * Set this as a Boolean.
   *
   * - value: Value.
   */
  function setBoolean(value:Boolean) {
    set().setBoolean(value);
  }

  /**
   * Set this as an integer.
   *
   * - value: Value.
   */
  function setInteger(value:Integer) {
    set().setInteger(value);
  }

  /**
   * Set this as a real.
   *
   * - value: Value.
   */
  function setReal(value:Real) {
    set().setReal(value);
  }

  /**
   * Set this as a string.
   *
   * - value: Value.
   */
  function setString(value:String) {
    set().setString(value);
  }

  /**
   * Set this as an object.
   *
   * - value: Value.
   */
  function setObject(value:Object) {
    set().setObject(value);
  }

  /**
   * Set this as a vector of Booleans.
   *
   * - value: Value.
   */
  function setBooleanVector(value:Boolean[_]) {
    set().setBooleanVector(value);
  }

  /**
   * Set this as a vector of integers.
   *
   * - value: Value.
   */
  function setIntegerVector(value:Integer[_]) {
    set().setIntegerVector(value);
  }

  /**
   * Set this as a vector of reals.
   *
   * - value: Value.
   */
  function setRealVector(value:Real[_]) {
    set().setRealVector(value);
  }

  /**
   * Set this as matrix of Booleans.
   *
   * - value: Value.
   */
  function setBooleanMatrix(value:Boolean[_,_]) {
    set().setBooleanMatrix(value);
  }

  /**
   * Set this as a matrix of integers.
   *
   * - value: Value.
   */
  function setIntegerMatrix(value:Integer[_,_]) {
    set().setIntegerMatrix(value);
  }

  /**
   * Set this as a matrix of reals.
   *
   * - value: Value.
   */
  function setRealMatrix(value:Real[_,_]) {
    set().setRealMatrix(value);
  }

  /**
   * Set child (to nil).
   */
  function setChild(name:String) -> Buffer;

  /**
   * Set child as an object.
   *
   * - name: Name of the child.
   */
  function setObject(name:String) {
    setChild(name).setObject();
  }
  
  /**
   * Set child as an array.
   *
   * - name: Name of the child.
   */
  function setArray(name:String) {
    setChild(name).setArray();
  }

  /**
   * Set child as nil.
   *
   * - name: Name of the child.
   */
  function setNil(name:String) {
    setChild(name).setNil();
  }

  /**
   * Set child as a Boolean.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setBoolean(name:String, value:Boolean) {
    setChild(name).setBoolean(value);
  }

  /**
   * Set child as an integer.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setInteger(name:String, value:Integer) {
    setChild(name).setInteger(value);
  }

  /**
   * Set child as a real.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setReal(name:String, value:Real) {
    setChild(name).setReal(value);
  }

  /**
   * Set child as a string.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setString(name:String, value:String) {
    setChild(name).setString(value);
  }

  /**
   * Set child as an object.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setObject(name:String, value:Object) {
    setChild(name).setObject(value);
  }

  /**
   * Set child as a vector of Booleans.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setBooleanVector(name:String, value:Boolean[_]) {
    setChild(name).setBooleanVector(value);
  }

  /**
   * Set child as a vector of integers.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setIntegerVector(name:String, value:Integer[_]) {
    setChild(name).setIntegerVector(value);
  }

  /**
   * Set child as a vector of reals.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setRealVector(name:String, value:Real[_]) {
    setChild(name).setRealVector(value);  
  }

  /**
   * Set child as a matrix of Booleans.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setBooleanMatrix(name:String, value:Boolean[_,_]) {
    setChild(name).setBooleanMatrix(value);  
  }

  /**
   * Set child as a matrix of integers.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setIntegerMatrix(name:String, value:Integer[_,_]) {
    setChild(name).setIntegerMatrix(value);  
  }

  /**
   * Set child as a matrix of reals.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function setRealMatrix(name:String, value:Real[_,_]) {
    setChild(name).setRealMatrix(value);  
  }
  
  /**
   * Set this as a Boolean.
   *
   * - value: Value.
   */
  function set(value:Boolean) {
    setBoolean(value);
  }

  /**
   * Set this as an integer.
   *
   * - value: Value.
   */
  function set(value:Integer) {
    setInteger(value);
  }

  /**
   * Set this as a real.
   *
   * - value: Value.
   */
  function set(value:Real) {
    setReal(value);
  }

  /**
   * Set this as a string.
   *
   * - value: Value.
   */
  function set(value:String) {
    setString(value);
  }
  
  /**
   * Write as an object.
   *
   * - o: The object.
   */
  function set(value:Object) {
    setObject(value);
  }

  /**
   * Set this as a vector of Booleans.
   *
   * - value: Value.
   */
  function set(value:Boolean[_]) {
    setBooleanVector(value);
  }

  /**
   * Set this as a vector of integers.
   *
   * - value: Value.
   */
  function set(value:Integer[_]) {
    setIntegerVector(value);
  }

  /**
   * Set this as a vector of reals.
   *
   * - value: Value.
   */
  function set(value:Real[_]) {
    setRealVector(value);
  }

  /**
   * Set this as matrix of Booleans.
   *
   * - value: Value.
   */
  function set(value:Boolean[_,_]) {
    setBooleanMatrix(value);
  }

  /**
   * Set this as a matrix of integers.
   *
   * - value: Value.
   */
  function set(value:Integer[_,_]) {
    setIntegerMatrix(value);
  }

  /**
   * Set this as a matrix of reals.
   *
   * - value: Value.
   */
  function set(value:Real[_,_]) {
    setRealMatrix(value);
  }

  /**
   * Set a Boolean.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Boolean) {
    setBoolean(name, value);
  }

  /**
   * Set an integer.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Integer) {
    setInteger(name, value);
  }

  /**
   * Set a real.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Real) {
    setReal(name, value);
  }

  /**
   * Set a string.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:String) {
    setString(name, value);
  }

  /**
   * Set an object.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Object) {
    setObject(name, value);
  }
  
  /**
   * Set a vector of Booleans.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Boolean[_]) {
    setBooleanVector(name, value);
  }

  /**
   * Set a vector of integers.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Integer[_]) {
    setIntegerVector(name, value);
  }

  /**
   * Set a vector of reals.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Real[_]) {
    setRealVector(name, value);
  }

  /**
   * Set a matrix of Booleans.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Boolean[_,_]) {
    setBooleanMatrix(name, value);
  }

  /**
   * Set a matrix of integers.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Integer[_,_]) {
    setIntegerMatrix(name, value);
  }

  /**
   * Set a matrix of reals.
   *
   * - name: Name of the child.
   * - value: Value.
   */
  function set(name:String, value:Real[_,_]) {
    setRealMatrix(name, value);
  }
  
  /**
   * Push a new element onto the end of an array.
   *
   * Returns: a buffer for modifying the new element.
   */
  function push() -> Buffer;
}
