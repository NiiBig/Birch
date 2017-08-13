/**
 * @file
 */
#pragma once

#include "bi/io/bih_ostream.hpp"

#include <list>

namespace bi {
/**
 * Output stream for Markdown files.
 *
 * @ingroup compiler_io
 */
class md_ostream: public bih_ostream {
public:
  md_ostream(std::ostream& base);

  using bih_ostream::visit;

  virtual void visit(const Package* o);
  virtual void visit(const Name* o);

  virtual void visit(const Parameter* o);
  virtual void visit(const GlobalVariable* o);
  virtual void visit(const MemberVariable* o);
  virtual void visit(const Function* o);
  virtual void visit(const Fiber* o);
  virtual void visit(const Program* o);
  virtual void visit(const MemberFunction* o);
  virtual void visit(const MemberFiber* o);
  virtual void visit(const BinaryOperator* o);
  virtual void visit(const UnaryOperator* o);
  virtual void visit(const AssignmentOperator* o);
  virtual void visit(const ConversionOperator* o);
  virtual void visit(const Class* o);

  virtual void visit(const ListType* o);
  virtual void visit(const ClassType* o);
  virtual void visit(const BasicType* o);
  virtual void visit(const AliasType* o);
  virtual void visit(const IdentifierType* o);
  virtual void visit(const ArrayType* o);
  virtual void visit(const ParenthesesType* o);
  virtual void visit(const FunctionType* o);
  virtual void visit(const FiberType* o);
  virtual void visit(const OptionalType* o);

private:
  void genHead(const std::string& name);

  template<class ObjectType, class RootType>
  void genBrief(const std::string& name, const RootType* root);

  template<class ObjectType, class RootType>
  void genOneLine(const std::string& name, const RootType* root);

  template<class ObjectType, class RootType>
  void genDetailed(const std::string& name, const RootType* root);

  template<class ObjectType, class RootType>
  void genSections(const std::string& name, const RootType* root);

  /**
   * Current section depth.
   */
  int depth;
};
}

#include "bi/visitor/Gatherer.hpp"
#include "bi/primitive/encode.hpp"

template<class ObjectType, class RootType>
void bi::md_ostream::genBrief(const std::string& name, const RootType* root) {
  Gatherer<ObjectType> gatherer;
  root->accept(&gatherer);
  if (gatherer.size() > 0) {
    genHead(name);
    line("| --- | --- |");
    ++depth;
    for (auto o : gatherer) {
      line("| *" << o << "* | " << brief(o->loc->doc) << " |");
    }
    line("");
    --depth;
  }
}

template<class ObjectType, class RootType>
void bi::md_ostream::genOneLine(const std::string& name,
    const RootType* root) {
  Gatherer<ObjectType> gatherer;
  root->accept(&gatherer);
  if (gatherer.size() > 0) {
    genHead(name);
    line("| --- | --- |");
    ++depth;
    for (auto o : gatherer) {
      line("| *" << o << "* | " << one_line(o->loc->doc) << " |");
    }
    line("");
    --depth;
  }
}

template<class ObjectType, class RootType>
void bi::md_ostream::genDetailed(const std::string& name,
    const RootType* root) {
  Gatherer<ObjectType> gatherer([](const ObjectType* o) {
        return !detailed(o->loc->doc).empty();
      });
  root->accept(&gatherer);
  if (gatherer.size() > 0) {
    genHead(name);
    ++depth;
    for (auto o : gatherer) {
      std::string desc = detailed(o->loc->doc);
      line("*" << o << "*\n");
      line(desc << "\n");
    }
    --depth;
  }
}

template<class ObjectType, class RootType>
void bi::md_ostream::genSections(const std::string& name,
    const RootType* root) {
  Gatherer<ObjectType> gatherer([](const ObjectType* o) {
        return !detailed(o->loc->doc).empty();
      });
  root->accept(&gatherer);
  if (gatherer.size() > 0) {
    genHead(name);
    ++depth;
    for (auto o : gatherer) {
      genHead(o->name->str());
      *this << o;
    }
    --depth;
  }
}
