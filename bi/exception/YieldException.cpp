/**
 * @file
 */
#include "bi/exception/YieldException.hpp"

#include "bi/io/bih_ostream.hpp"

#include <sstream>

bi::YieldException::YieldException(const Yield* o) {
  std::stringstream base;
  bih_ostream buf(base);
  if (o->loc) {
    buf << o->loc;
  }
  buf << "error: yield outside of fiber\n";
  if (o->loc) {
    buf << o->loc;
  }
  buf << "note: in\n";
  buf << o;
  msg = base.str();
}
