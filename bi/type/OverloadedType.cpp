/**
 * @file
 */
#include "bi/type/OverloadedType.hpp"

#include "bi/visitor/all.hpp"
#include "bi/exception/all.hpp"

bi::OverloadedType::OverloadedType(Overloaded* overloaded, Location* loc,
    const bool assignable) :
    Type(loc, assignable),
    overloaded(overloaded) {
  //
}

bi::OverloadedType::~OverloadedType() {
  //
}

bool bi::OverloadedType::isOverloaded() const {
  return true;
}

bi::FunctionType* bi::OverloadedType::resolve(Argumented* args) {
  std::list<Parameterised*> matches;
  overloaded->overloads.match(args, matches);
  if (matches.size() == 1) {
    /* construct the appropriate function type */
    auto target = matches.front();
    Type* paramsType = target->params->type;
    Type* returnType = dynamic_cast<ReturnTyped*>(target)->returnType;
    return new FunctionType(paramsType, returnType);
  } else if (matches.size() == 0) {
    std::list<Parameterised*> available;
    std::copy(overloaded->overloads.begin(), overloaded->overloads.end(),
        std::back_inserter(available));
    throw CallException(args, available);
  } else {
    throw AmbiguousCallException(args, matches);
  }
}

bi::Type* bi::OverloadedType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::OverloadedType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::OverloadedType::accept(Visitor* visitor) const {
  return visitor->visit(this);
}

bool bi::OverloadedType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::OverloadedType::definitely(const OverloadedType& o) const {
  return false;
}

bool bi::OverloadedType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::OverloadedType::possibly(const OverloadedType& o) const {
  return false;
}
