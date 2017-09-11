import basic;
import math;

/**
 * Output stream.
 */
class OutputStream(stream:File) {
  /**
   * Print with format. See system `printf`.
   */
  function printf(fmt:String, value:Boolean) {
    cpp {{
    ::fprintf(stream_, fmt_.c_str(), value_);
    }}
  }

  /**
   * Print with format. See system `printf`.
   */
  function printf(fmt:String, value:Integer) {
    cpp {{
    ::fprintf(stream_, fmt_.c_str(), value_);
    }}
  }

  /**
   * Print with format. See system `printf`.
   */
  function printf(fmt:String, value:Real) {
    cpp {{
    ::fprintf(stream_, fmt_.c_str(), value_);
    }}
  }

  /**
   * Print with format. See system `printf`.
   */
  function printf(fmt:String, value:String) {
    cpp {{
    ::fprintf(stream_, fmt_.c_str(), value_.c_str());
    }}
  }

  /**
   * Print scalar.
   */
  function print(value:Boolean) {
    if (value) {
      print("true");
    } else {
      print("false");
    }
  }

  /**
   * Print scalar.
   */
  function print(value:Integer) {
    printf("%d", value);
  }

  /**
   * Print scalar.
   */
  function print(value:Real) {
    printf("%f", value);
  }

  /**
   * Print scalar.
   */
  function print(value:String) {
    printf("%s", value);
  }

  /**
   * Print vector.
   */
  function print(x:Integer[_]) {
    for (i:Integer in 1..length(x)) {
      if (i != 1) {
        print(", ");
      }
      print(x[i]);
    }
  }

  /**
   * Print vector.
   */
  function print(x:Real[_]) {
    for (i:Integer in 1..length(x)) {
      if (i != 1) {
        print(" ");
      }
      print(x[i]);
    }
  }

  /**
   * Print matrix.
   */
  function print(X:Integer[_,_]) {
    for (i:Integer in 1..rows(X)) {
      for (j:Integer in 1..columns(X)) {
        if (j != 1) {
          print("  ");
        }
        print(X[i,j]);
      }
      print("\n");
    }
  }

  /**
   * Print matrix.
   */
  function print(X:Real[_,_]) {
    for (i:Integer in 1..rows(X)) {
      for (j:Integer in 1..columns(X)) {
        if (j != 1) {
          print(" ");
        }
        print(X[i,j]);
      }
      print("\n");
    }
  }
}
