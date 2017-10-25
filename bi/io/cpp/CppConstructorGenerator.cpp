/**
 * @file
 */
#include "bi/io/cpp/CppConstructorGenerator.hpp"

bi::CppConstructorGenerator::CppConstructorGenerator(std::ostream& base,
    const int level, const bool header) :
    CppBaseGenerator(base, level, header) {
  //
}

void bi::CppConstructorGenerator::visit(const Class* o) {
  if (!header) {
    genTemplateParams(o);
    start("bi::" << o->name);
    genTemplateArgs(o);
    middle("::");
  } else {
    start("");
  }
  middle(o->name);
  CppBaseGenerator aux(base, level, header);
  aux << '(' << o->params << ')';
  if (header) {
    finish(";\n");
  } else {
    finish(" :");
    in();
    in();
    start("super_type(");
    if (!o->baseArgs->isEmpty()) {
      middle(o->baseArgs);
    }
    middle(')');
    for (auto iter = o->params->begin(); iter != o->params->end(); ++iter) {
      *this << *iter;
    }
    *this << o->braces->strip();
    out();
    out();
    finish(" {");
    in();
    line("//");
    out();
    line("}\n");
  }
}

void bi::CppConstructorGenerator::visit(const MemberParameter* o) {
  finish(',');
  start(o->name << '(' << o->name << ')');
}

void bi::CppConstructorGenerator::visit(const MemberVariable* o) {
  if (o->type->isClass()) {
    finish(',');
    start(o->name << '(');
    const Named* named = dynamic_cast<const Named*>(o->type);
    assert(named);
    middle("bi::make_object<" << named->name << ">(" << o->parens << ')');
    middle(')');
  } else if (o->type->isArray()) {
    finish(',');
    start(o->name << '(');
    const ArrayType* type = dynamic_cast<const ArrayType*>(o->type);
    assert(type);
    middle("bi::make_frame(" << type->brackets << ")");
    if (!o->parens->isEmpty()) {
      middle(", " << o->parens);
    }
    middle(')');
  } else if (!o->value->isEmpty()) {
    finish(',');
    start(o->name << '(' << o->value << ')');
  }
}

void bi::CppConstructorGenerator::visit(const MemberFunction* o) {
  //
}

void bi::CppConstructorGenerator::visit(const MemberFiber* o) {
  //
}

void bi::CppConstructorGenerator::visit(const ConversionOperator* o) {
  //
}

void bi::CppConstructorGenerator::visit(const AssignmentOperator* o) {
  //
}
