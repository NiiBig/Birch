/**
 * @file
 */
#pragma once

#include <string>

namespace bi {
/**
 * Exception.
 *
 * @ingroup birch_exception
 */
struct Exception {
  /**
   * Default constructor.
   */
  Exception();

  /**
   * Constructor.
   */
  Exception(const std::string& msg);

  /**
   * Message.
   */
  std::string msg;
};
}
