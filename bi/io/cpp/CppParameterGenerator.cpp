/**
 * @file
 */
#include "bi/io/cpp/CppParameterGenerator.hpp"

#include "bi/visitor/Gatherer.hpp"

bi::CppParameterGenerator::CppParameterGenerator(std::ostream& base,
    const int level, const bool header) :
    CppBaseGenerator(base, level, header) {
  //
}

void bi::CppParameterGenerator::visit(const VarParameter* o) {
  if (!o->type->assignable && !inArray) {
    middle("const ");
  }
  middle(o->type << "& " << o->name);
  if (!o->value->isEmpty()) {
    middle(" = " << o->value);
  }
}

void bi::CppParameterGenerator::visit(const FuncParameter* o) {
  Gatherer<VarParameter> gatherer;
  o->parens->accept(&gatherer);

  middle('(');
  for (auto iter = gatherer.begin(); iter != gatherer.end(); ++iter) {
    if (iter != gatherer.begin()) {
      middle(", ");
    }
    middle(*iter);
  }
  middle(')');
}
