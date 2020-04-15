/**
 * @file
 */
#pragma once

#include "libbirch/Tuple.hpp"
#include "libbirch/Array.hpp"
#include "libbirch/Optional.hpp"
#include "libbirch/Fiber.hpp"
#include "libbirch/Lazy.hpp"
#include "libbirch/Finisher.hpp"
#include "libbirch/Freezer.hpp"
#include "libbirch/memory.hpp"

namespace libbirch {
/**
 * Visitor for relabelling members of a newly copied object.
 *
 * @ingroup libbirch
 */
class Copier {
public:
  /**
   * Constructor.
   */
  Copier(Label* label) :
      label(label) {
    //
  }

  /**
   * Visit empty list of variables (base case).
   */
  void visit() const {
    //
  }

  /**
   * Visit list of variables.
   *
   * @param arg First variable.
   * @param args... Remaining variables.
   */
  template<class Arg, class... Args>
  void visit(Arg& arg, Args&... args) const {
    visit(arg);
    visit(args...);
  }

  /**
   * Visit a value.
   */
  template<class T, std::enable_if_t<is_value<T>::value && std::is_trivially_copy_constructible<T>::value,int> = 0>
  void visit(T& arg) const {
    //
  }

  /**
   * Visit a value.
   */
  template<class T, std::enable_if_t<is_value<T>::value && !std::is_trivially_copy_constructible<T>::value,int> = 0>
  void visit(T& arg) const {
    /* for types that do not support trivial copy, the bitwise copy is
     * invalid; we correct for this now by first performing a proper copy,
     * then emplacing the result over the bitwise copy */
    T proper(arg);
    new (&arg) T(std::move(proper));
  }

  /**
   * Visit a tuple.
   */
  template<class Head, class... Tail>
  void visit(Tuple<Head,Tail...>& o) const {
    o.accept_(*this);
  }

  /**
   * Visit an array of non-value type.
   */
  template<class T, class F>
  void visit(Array<T,F>& o) const {
    o.bitwiseFix();
    o.accept_(*this);
  }

  /**
   * Visit an optional of non-value type.
   */
  template<class T>
  void visit(Optional<T>& o) const {
    o.accept_(*this);
  }

  /**
   * Visit a fiber.
   */
  template<class Yield, class Return>
  void visit(Fiber<Yield,Return>& o) const {
    o.accept_(*this);
  }

  /**
   * Visit a lazy pointer.
   */
  template<class P>
  void visit(Lazy<P>& o) const {
    o.bitwiseFix();
    o.setLabel(label);
  }

  /**
   * Label associated with the clone.
   */
  Label* label;
};

/**
 * Clone an object via a pointer.
 *
 * @ingroup libbirch
 *
 * @param o The pointer.
 */
template<class P>
auto clone(const Lazy<P>& o) {
  auto object = o.pull();
  auto oldLabel = o.getLabel();

  finishLock.enter();
  object->finish(oldLabel);
  finishLock.exit();

  freezeLock.enter();
  object->freeze();
  freezeLock.exit();

  auto newLabel = new Label(*oldLabel);

  finishLock.enter();
  newLabel->finish(oldLabel);
  finishLock.exit();

  freezeLock.enter();
  newLabel->freeze();
  freezeLock.exit();

  Lazy<P> ptr(object, newLabel);
  ptr.get();
  return ptr;
}

}
