/**
 * @file
 */
#pragma once

#include "bi/visitor/Visitor.hpp"

#include <set>

namespace bi {
/**
 * Gather all possible matches for function references that will need to be
 * checked at runtime.
 *
 * @ingroup compiler_visitor
 */
class DispatchGatherer : public Visitor {
public:
  /**
   * Begin iterator over gathered objects.
   */
  auto begin() {
    return gathered.begin();
  }

  /**
   * End iterator over gathered objects.
   */
  auto end() {
    return gathered.end();
  }

  /**
   * Number of items gathered.
   */
  auto size() {
    return gathered.size();
  }

  using Visitor::visit;
  virtual void visit(const FuncReference* o);
  virtual void visit(const RandomInit* o);

private:
  /**
   * Gathered matches.
   */
  std::set<const Dispatcher*> gathered;
};
}
