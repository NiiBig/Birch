/**
 * @file
 */
#pragma once

#include "bi/io/cpp/CppFiberGenerator.hpp"

namespace bi {
/**
 * C++ code generator for member Fibers.
 *
 * @ingroup compiler_io
 */
class CppMemberFiberGenerator: public CppFiberGenerator {
public:
  CppMemberFiberGenerator(const Class* type, std::ostream& base,
      const int level = 0, const bool header = false);

  using indentable_ostream::visit;

  virtual void visit(const MemberFiber* o);
  virtual void visit(const Identifier<MemberParameter>* o);
  virtual void visit(const Identifier<MemberVariable>* o);
  virtual void visit(const OverloadedIdentifier<MemberFunction>* o);
  virtual void visit(const OverloadedIdentifier<MemberFiber>* o);
  virtual void visit(const Member* o);
  virtual void visit(const This* o);

private:
  /**
   * The class being generated.
   */
  const Class* type;

  /**
   * Current state index.
   */
  int state;

  /**
   * Are we in a membership expression?
   */
  int inMember;
};
}
