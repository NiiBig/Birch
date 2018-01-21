/**
 * @file
 */
#pragma once

#include "bi/exception/CompilerException.hpp"
#include "bi/expression/Member.hpp"

namespace bi {
/**
 * Incorrect use of '.' operator
 *
 * @ingroup exception
 */
struct MemberException: public CompilerException {
  /**
   * Constructor.
   */
  MemberException(const Member* expr);
};
}
