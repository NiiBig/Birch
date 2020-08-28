/**
 * @file
 */
#pragma once

#include "bi/build/misc.hpp"
#include "bi/statement/all.hpp"
#include "bi/exception/all.hpp"

namespace bi {
/**
 * Compiler.
 *
 * @ingroup driver
 */
class Compiler {
public:
  /**
   * Constructor.
   *
   * @param package The package.
   * @param mode Build mode.
   * @param unit Compilation unit.
   */
  Compiler(Package* package, const std::string& mode,
      const std::string& unit);

  /**
   * Parse source files.
   * 
   * @param includeHeaders Include header files also?
   */
  void parse(bool includeHeaders);

  /**
   * Resolve identifiers and check types.
   */
  void resolve();

  /**
   * Generate output code for all input files.
   */
  void gen();

  /**
   * Set the root statement of the current file.
   */
  void setRoot(Statement* root);

  /**
   * Current file being parsed (needed by GNU Bison parser).
   */
  File* file;

  /**
   * Root scope.
   */
  Scope* scope;

private:
  /**
   * Package.
   */
  Package* package;

  /**
   * Build mode.
   */
  std::string mode;

  /**
   * Compilation unit.
   */
  std::string unit;
};
}

extern bi::Compiler* compiler;
extern std::stringstream raw;
