/**
 * Two-dimensional array where each row has a varying length. Internally, this
 * is stored in one contiguous array for $O(1)$ random access.
 */
class RaggedArray<Type> {
  /**
   * Elements.
   */
  values:Type[_];

  /**
   * Offset into `values` for each row.
   */
  offsets:Integer[_];
  
  /**
   * Number of columns in each row.
   */
  ncols:Integer[_];
  
  /**
   * Number of rows.
   */
  nrows:Integer <- 0;
  
  /**
   * Number of elements.
   */
  nelements:Integer <- 0;

  /**
   * Is this empty?
   */
  function empty() -> Boolean {
    return nelements == 0;
  }

  /**
   * Clear all elements.
   */
  function clear() {
    shrink(0);
  }

  /**
   * Number of rows.
   */
  function size() -> Integer {
    return nrows;
  }
  
  /**
   * Number of elements for a given row.
   *
   * - i: Row.
   */
  function size(i:Integer) -> Integer {
    assert 0 < i && i <= nrows;
    return ncols[i];
  }
  
  /**
   * Get an element.
   *
   * - i: Row.
   * - j: Column.
   */
  function get(i:Integer, j:Integer) -> Type {
    return values[serial(i, j)];
  }

  /**
   * Get a row.
   *
   * - i: Row.
   */
  function get(i:Integer) -> Type[_] {
    assert 0 < i && i <= nrows;
    return values[from(i)..to(i)];
  }

  /**
   * Set an element.
   *
   * - i: Row.
   * - j: Column.
   * - x: Value.
   */
  function set(i:Integer, j:Integer, x:Type) {
    values[serial(i, j)] <- x;
  }

  /**
   * Set a row.
   *
   * - i: Row.
   * - x: Values.
   *
   * The number of columns in the row must match the number of columns in
   * `x`. Use `shrink` or `enlarge` beforehand if necessary.
   */
  function set(i:Integer, x:Type[_]) {
    values[from(i)..to(i)] <- x;
  } 

  /**
   * Add a new row at the back.
   */
  function pushBack() {
    enlarge(nrows + 1);
  }

  /**
   * Add a new element to the end of a row.
   *
   * - i: Row.
   * - x: Value.
   */
  function pushBack(i:Integer, x:Type) {
    enlarge(i, ncols[i] + 1, x);
  }

  /**
   * Iterate over the rows.
   *
   * Return: a fiber object that yields each row in forward order.
   */
  fiber walk() -> Type[_] {
    for (i:Integer in 1..nrows) {
      yield get(i);
    }
  }

  /**
   * Decrease the number of rows.
   *
   * - n: Number of rows.
   *
   * The current contents is preserved.
   */
  function shrink(n:Integer) {
    assert n < nrows;

    cpp{{
    offsets_.shrink(bi::make_frame(n_));
    ncols_.shrink(bi::make_frame(n_));
    }}
    if (n == 0) {
      nelements <- 0;
    } else {
      nelements <- offsets[n] + ncols[n] - 1;
    }
    nrows <- n;
    
    assert length(offsets) == length(ncols);
  }

  /**
   * Decrease the number of columns in a row.
   *
   * - i: Row.
   * - n: Number of columns.
   *
   * The current contents of the row is preserved.
   */
  function shrink(i:Integer, n:Integer) {
    assert n < ncols[i];
  
    d:Integer <- ncols[i] - n;
    values[(offsets[i] + n)..(nelements - d)] <- values[(offsets[i] + n + d)..nelements];
    nelements <- nelements - d;
    cpp{{
    values_.shrink(bi::make_frame(nelements_));
    }}
  }

  /**
   * Increase the number of rows.
   *
   * - n: Number of rows.
   *
   * The current contents is preserved.
   */
  function enlarge(n:Integer) {
    assert n > nrows;

    cpp{{    
    offsets_.enlarge(bi::make_frame(n_), nelements_ + 1);
    ncols_.enlarge(bi::make_frame(n_), 0);
    }}
    nrows <- n;

    assert length(offsets) == length(ncols);
  }
  
  /**
   * Increase the number of columns in a row.
   *
   * - i: Row.
   * - n: Number of columns.
   * - x: Value for new elements.
   *
   * The current contents of the row is preserved.
   */
  function enlarge(i:Integer, n:Integer, x:Type) {
    assert n > ncols[i];
  
    d:Integer <- n - ncols[i];
    cpp{{
    values_.enlarge(bi::make_frame(nelements_ + d_), x_);
    }}
    values[(offsets[i] + n)..(nelements + d)] <- values[(offsets[i] + n - d)..nelements];
    for (j:Integer in (offsets[i] + n - d)..(offsets[i] + n - 1)) {
      values[j] <- x;
    }
    ncols[i] <- n;
    offsets[(i + 1)..nrows] <- offsets[(i + 1)..nrows] + d;
    nelements <- nelements + d;
  }

  /**
   * First serial index of a row.
   *
   * - i: Row.
   */
  function from(i:Integer) -> Integer {
    assert 0 < i && i <= nrows;
    return offsets[i];
  }
  
  /**
   * Last serial index of a row.
   *
   * - i: Row.
   */
  function to(i:Integer) -> Integer {
    assert 0 < i && i <= nrows;
    return offsets[i] + ncols[i] - 1;
  }
  
  /**
   * Serial index of row and column.
   *
   * - i: Row.
   * - j: Column.
   */
  function serial(i:Integer, j:Integer) -> Integer {
    assert 0 < i && i <= nrows;
    assert 0 < j && j <= ncols[i];
    return offsets[i] + j - 1;
  }
}
