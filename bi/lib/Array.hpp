/**
 * @file
 */
#pragma once

#include "bi/lib/Frame.hpp"
#include "bi/lib/Iterator.hpp"
#include "bi/lib/constant.hpp"
#include "bi/lib/memory.hpp"
#include "bi/lib/Pointer.hpp"

#include <cstring>

namespace bi {
/**
 * Array. Combines underlying data and a frame describing the shape of that
 * data. Allows the construction of views of the data, where a view indexes
 * either an individual element or some range of elements.
 *
 * @ingroup library
 *
 * @tparam Type Value type.
 * @tparam Frame Frame type.
 */
template<class Type, class Frame = EmptyFrame>
class Array {
  template<class Type1, class Frame1>
  friend class Array;

  /**
   * @internal These are declare ahead due to some issues with clang++ around
   * the return type of operator(), which in turn calls viewReturn().
   */
protected:
  /**
   * Frame.
   */
  Frame frame;

  /**
   * Value.
   */
  Pointer<Type> ptr;

  /**
   * Copy from another array.
   */
  template<class Frame1>
  void copy(const Array<Type,Frame1>& o) {
    /* pre-condition */
    assert(frame.conforms(o.frame));

    size_t block1 = frame.block();
    auto iter1 = begin();
    auto end1 = end();

    size_t block2 = o.frame.block();
    auto iter2 = o.begin();
    auto end2 = o.end();

    size_t block = gcd(block1, block2);
    for (; iter1 != end1; iter1 += block, iter2 += block) {
      std::memcpy(&(*iter1), &(*iter2), block * sizeof(Type));
    }
    assert(iter2 == end2);
  }

  /**
   * Return value of view when result is an array.
   */
  template<class Frame1>
  Array<Type,Frame1> viewReturn(const Pointer<Type>& ptr,
      const Frame1& frame) {
    return Array<Type,Frame1>(ptr, frame);
  }
  template<class Frame1>
  Array<Type,Frame1> viewReturn(const Pointer<Type>& ptr,
      const Frame1& frame) const {
    return Array<Type,Frame1>(ptr, frame);
  }

  /**
   * Return value of view when result is a scalar.
   */
  Type& viewReturn(const Pointer<Type>& ptr, const EmptyFrame& frame) {
    return *ptr;
  }
  const Type& viewReturn(const Pointer<Type>& ptr,
      const EmptyFrame& frame) const {
    return *ptr;
  }

public:
  /**
   * Constructor with new allocation.
   *
   * @tparam ...Args Arbitrary types.
   *
   * @param frame Frame.
   * @param args Constructor arguments.
   *
   * Memory is allocated for the array, and is freed on destruction. After
   * allocation, the constructor is called for each element with the given
   * arguments.
   */
  template<class ... Args>
  Array(const Frame& frame, Args ... args) :
      frame(frame) {
    create(ptr, frame.volume() * sizeof(Type));
    fill(args...);
  }

  /**
   * Constructor with existing allocation.
   *
   * @tparam Frame Frame type.
   *
   * @param ptr Existing allocation.
   * @param frame Frame.
   */
  Array(const Pointer<Type>& ptr, const Frame& frame) :
      frame(frame),
      ptr(ptr) {
    //
  }

  /**
   * Copy constructor.
   */
  Array(const Array<Type,Frame>& o) :
      frame(o.frame) {
    create(ptr, frame.volume() * sizeof(Type));
    fill();
    *this = o;
  }

  /**
   * Move constructor.
   */
  Array(Array<Type,Frame> && o) = default;

  /**
   * Copy assignment. The frames of the two arrays must conform.
   */
  Array<Type,Frame>& operator=(const Array<Type,Frame>& o) {
    /* pre-condition */
    assert(frame.conforms(o.frame));

    if (ptr != o.ptr) {
      copy(o);
    }
    return *this;
  }

  /**
   * Move assignment. The frames of the two arrays must conform.
   */
  Array<Type,Frame>& operator=(Array<Type,Frame> && o) {
    /* pre-condition */
    assert(frame.conforms(o.frame));

    if (ptr != o.ptr) {
      copy(o);
    }
    return *this;
  }

  /**
   * Generic assignment. The frames of the two arrays must conform.
   */
  template<class Frame1>
  Array<Type,Frame>& operator=(const Array<Type,Frame1>& o) {
    /* pre-condition */
    assert(frame.conforms(o.frame));

    copy(o);
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
   *
   * @internal The decltype use for the return type here seems necessary,
   * clang++ is otherwise giving these a const return type.
   */
  template<class View1>
  auto operator()(
      const View1& view) -> decltype(viewReturn(ptr + frame.serial(view), this->frame(view))) {
    return viewReturn(ptr + frame.serial(view), frame(view));
  }

  /**
   * View operator.
   */
  template<class View1>
  auto operator()(
      const View1& view) const -> decltype(viewReturn(ptr + frame.serial(view), frame(view))) {
    return viewReturn(ptr + frame.serial(view), frame(view));
  }

  /**
   * @name Selections
   */
  //@{
  /**
   * Access the first element.
   */
  Type& front() {
    return *ptr;
  }

  /**
   * Access the first element.
   */
  const Type& front() const {
    return *ptr;
  }

  /**
   * Access the last element.
   */
  Type& back() {
    return ptr + frame.volume() - 1;
  }

  /**
   * Access the last element.
   */
  const Type& back() const {
    return ptr + frame.volume() - 1;
  }
  //@}

  /**
   * @name Queries
   */
  //@{
  /**
   * Get the length of the @p i th dimension.
   */
  size_t length(const int i) const {
    return frame.length(i);
  }

  /**
   * Get the stride of the @p i th dimension.
   */
  ptrdiff_t stride(const int i) const {
    return frame.stride(i);
  }

  /**
   * Get the lead of the @p i th dimension.
   */
  size_t lead(const int i) const {
    return frame.lead(i);
  }
  //@}

  /**
   * @name Collections
   */
  //@{
  /**
   * Get lengths.
   *
   * @tparam Integer Integer type.
   *
   * @param[out] out Array assumed to have at least count() elements.
   */
  template<class Integer>
  void lengths(Integer* out) const {
    frame.lengths(out);
  }

  /**
   * Get strides.
   *
   * @tparam Integer Integer type.
   *
   * @param[out] out Array assumed to have at least count() elements.
   */
  template<class Integer>
  void strides(Integer* out) const {
    frame.strides(out);
  }

  /**
   * Get leads.
   *
   * @tparam Integer Integer type.
   *
   * @param[out] out Array assumed to have at least count() elements.
   */
  template<class Integer>
  void leads(Integer* out) const {
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
   */
  //@{
  /**
   * Iterator pointing to the first element.
   *
   * Iterators are used to access the elements of an array sequentially.
   * Elements are visited in the order in which they are stored in memory;
   * the rightmost dimension is the fastest moving (for a matrix, this is
   * "row major" order).
   *
   * The idiom of iterator usage is as for the STL.
   */
  Iterator<Type,Frame> begin() {
    return Iterator<Type,Frame>(ptr, frame);
  }

  /**
   * Iterator pointing to the first element.
   */
  Iterator<const Type,Frame> begin() const {
    return Iterator<const Type,Frame>(ptr, frame);
  }

  /**
   * Iterator pointing to one beyond the last element.
   */
  Iterator<Type,Frame> end() {
    return begin() + frame.size();
  }

  /**
   * Iterator pointing to one beyond the last element.
   */
  Iterator<const Type,Frame> end() const {
    return begin() + frame.size();
  }

  /**
   * Raw pointer to underlying buffer.
   */
  Type* buf() {
    return ptr;
  }

  /**
   * Raw pointer to underlying buffer.
   */
  Type* const buf() const {
    return ptr;
  }

private:
  template<class ... Args>
  void fill(Args ... args) {
    for (auto iter = begin(); iter != end(); ++iter) {
      construct(*iter, args...);
    }
  }
};

/**
 * Default array for `D` dimensions.
 */
template<class Type, int D>
using DefaultArray = Array<Type,typename DefaultFrame<D>::type>;
}
