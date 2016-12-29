/**
 * @file
 */
#pragma once

#include "bi/io/cpp/CppBaseGenerator.hpp"

namespace bi {
/**
 * C++ code generator for formal parameters.
 *
 * @ingroup compiler_io
 */
class CppParameterGenerator: public CppBaseGenerator {
public:
  CppParameterGenerator(std::ostream& base, const int level = 0,
      const bool header = false);

  virtual void visit(const ModelReference* o);
  virtual void visit(const FuncParameter* o);

protected:
  /**
   * Number of parameters.
   */
  int n;
};
}
