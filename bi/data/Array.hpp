/**
 * @file
 */
#pragma once

#include "bi/data/Frame.hpp"

namespace bi {
/**
 * Array. Combines underlying data and a frame describing the shape of that
 * data. Allows the construction of views of the data, where a view indexes
 * either an individual element or some range of elements.
 *
 * @ingroup library
 *
 * @tparam Value Value type.
 * @tparam Frame Frame type.
 */
template<class Value, class Frame = EmptyFrame>
class Array {
public:
  typedef typename Value::group_type group_type;

  /**
   * Constructor for new data.
   *
   * @tparam ...Args Arbitrary types.
   *
   * @param frame Frame.
   * @param name Optional name.
   * @param group Optional group.
   * @param args Optional constructor arguments.
   *
   * Memory is allocated for the array, and is freed on destruction.
   */
  template<class ... Args>
  Array(const Frame& frame, const char* name = nullptr,
      const group_type& group = group_type(), Args ... args) :
      value(args..., frame, name, group),
      frame(frame) {
    //
  }

  /**
   * Constructor for existing data.
   *
   * @tparam Frame1 Frame type.
   *
   * @param value Value.
   * @param frame Frame.
   *
   * Memory is not allocated for the array.
   */
  template<class Frame1>
  Array(const Value& value, const Frame1& frame) :
      value(value),
      frame(frame) {
    //
  }

  /**
   * View constructor.
   *
   * @tparam Frame1 Frame type.
   * @tparam View1 View type.
   *
   * @param o Array.
   * @param frame Extra frame.
   * @param view Extra view.
   */
  template<class Frame1, class View1>
  Array(const Array<Value,Frame>& o, const Frame1& frame, const View1& view) :
      value(o.value, frame, view),
      frame(o.frame) {
    //
  }

  /**
   * Shallow copy constructor.
   *
   * @param o Object.
   * @param offset Offset into the underlying buffer of @p o.
   */
  Array(const Array<Value,Frame>& o) :
      value(o.value),
      frame(o.frame) {
    //
  }

  /**
   * Generic shallow copy constructor.
   */
  template<class Frame1>
  Array(Array<Value,Frame1>& o) :
      value(o.value),
      frame(o.frame) {
    //
  }

  /**
   * Move constructor.
   */
  Array(Array<Value,Frame> && o) :
      value(o.value),
      frame(o.frame) {
    //
  }

  /**
   * Destructor.
   */
  ~Array() {
    //
  }

  /**
   * Deep assignment. The frames of the two arrays must conform.
   */
  Array<Value,Frame>& operator=(const Array<Value,Frame>& o) {
    copy(*this, o);
    return *this;
  }

  /**
   * Generic deep assignment. The frames of the two arrays must conform.
   */
  template<class Value1, class Frame1>
  Array<Value,Frame>& operator=(const Array<Value1,Frame1>& o) {
    copy(*this, o);
    return *this;
  }

  /**
   * View operator.
   *
   * @tparam View1 View type.
   *
   * @param o View.
   *
   * @return The new array.
   */
  template<class View1>
  auto operator()(const View1& view) {
    return viewReturn(value(frame, view), frame(view));
  }

  /**
   * View operator.
   */
  template<class View1>
  auto operator()(const View1& view) const {
    return viewReturn(value(frame, view), frame(view));
  }

  /**
   * @name Selections
   */
  //@{
  /**
   * Access the first element.
   */
  Value& front() {
    return value;
  }

  /**
   * Access the first element.
   */
  const Value& front() const {
    return value;
  }

  /**
   * Access the last element.
   */
  Value& back() {
    return value + (frame.lead - 1);
  }

  /**
   * Access the last element.
   */
  const Value& back() const {
    return value + (frame.lead - 1);
  }
  //@}

  /**
   * @name Collections
   */
  //@{
  /**
   * Get lengths.
   *
   * @tparam T1 Integer type.
   *
   * @param[out] out Array assumed to have at least count() elements.
   */
  template<class T1>
  void lengths(T1* out) const {
    frame.lengths(out);
  }

  /**
   * Get strides.
   *
   * @tparam T1 Integer type.
   *
   * @param[out] out Array assumed to have at least count() elements.
   */
  template<class T1>
  void strides(T1* out) const {
    frame.strides(out);
  }

  /**
   * Get leads.
   *
   * @tparam T1 Integer type.
   *
   * @param[out] out Array assumed to have at least count() elements.
   */
  template<class T1>
  void leads(T1* out) const {
    frame.leads(out);
  }
  //@}

  /**
   * @name Reductions
   */
  //@{
  /**
   * Number of spans in the frame.
   */
  static constexpr int count() {
    return Frame::count();
  }

  /**
   * Are all elements stored contiguously in memory?
   */
  bool contiguous() const {
    return frame.contiguous();
  }
  //@}

  /**
   * @name Iteration
   *
   */
  //@{
  /**
   * Iterator pointing to the first element.
   *
   * Iterators are used to access the elements of an array sequentially.
   * Elements are visited in the order in which they are stored in memory;
   * the leftmost dimension is the fastest moving (for a matrix, this is
   * "column major" order).
   *
   * The idiom of iterator usage is as for the STL.
   */
  template<class View = typename DefaultView<Frame::count()>::type>
  auto begin(const View& view = typename DefaultView<Frame::count()>::type());

  /**
   * Iterator pointing to the first element.
   */
  template<class View = typename DefaultView<Frame::count()>::type>
  auto begin(const View& view =
      typename DefaultView<Frame::count()>::type()) const;

  /**
   * Iterator pointing to one beyond the last element.
   */
  template<class View = typename DefaultView<Frame::count()>::type>
  auto end(const View& view = typename DefaultView<Frame::count()>::type());

  /**
   * Iterator pointing to one beyond the last element.
   */
  template<class View = typename DefaultView<Frame::count()>::type>
  auto end(const View& view =
      typename DefaultView<Frame::count()>::type()) const;

//protected:
  /**
   * Value.
   */
  Value value;

  /**
   * Frame.
   */
  Frame frame;

  /**
   * View return.
   */
  template<class Value1, class Frame1>
  auto viewReturn(const Value1& value, const Frame1& frame) const {
    return Array<Value1,Frame1>(value, frame);
  }
  template<class Value1>
  auto viewReturn(const Value1& value, const EmptyFrame& frame) const {
    return value;
  }
};

/**
 * Default array for `D` dimensions.
 */
template<class Value, int D>
using DefaultArray = Array<Value,typename DefaultFrame<D>::type>;
}

#include "bi/data/Iterator.hpp"

template<class Value, class Frame>
template<class View>
auto bi::Array<Value,Frame>::begin(const View& view) {
  return Iterator<Value,Frame,View>(value, frame, view);
}

template<class Value, class Frame>
template<class View>
auto bi::Array<Value,Frame>::begin(const View& view) const {
  return Iterator<const Value,Frame,View>(value, frame, view);
}

template<class Value, class Frame>
template<class View>
auto bi::Array<Value,Frame>::end(const View& view) {
  return Iterator<Value,Frame,View>(value, frame, view, frame.length);
}

template<class Value, class Frame>
template<class View>
auto bi::Array<Value,Frame>::end(const View& view) const {
  return Iterator<const Value,Frame,View>(value, frame, view, frame.length);
}
