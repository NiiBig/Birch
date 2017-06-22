/**
 * @file
 */
#include "bi/io/cpp/CppConstructorGenerator.hpp"

bi::CppConstructorGenerator::CppConstructorGenerator(std::ostream& base,
    const int level, const bool header) :
    CppBaseGenerator(base, level, header),
    before(false) {
  //
}

void bi::CppConstructorGenerator::visit(const Class* o) {
  if (!header) {
    start("bi::type::" << o->name << "::");
  } else {
    start("");
  }
  middle(o->name);
  if (o->parens->isEmpty()) {
    middle("()");
  } else {
    CppBaseGenerator aux(base, level, header);
    aux << o->parens;
  }
  if (header) {
    finish(";\n");
  } else {
    finish(" :");
    in();
    in();
    if (!o->base->isEmpty() && !o->baseParens->isEmpty()) {
      before = true;
      start("super_type" << o->baseParens);
    }
    for (auto iter = o->parens->begin(); iter != o->parens->end(); ++iter) {
      *this << *iter;
    }
    *this << o->braces;
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
  if (before) {
    finish(',');
  }
  before = true;
  start(o->name << '(' << o->name << ')');
}

void bi::CppConstructorGenerator::visit(const MemberVariable* o) {
  if (before) {
    finish(',');
  }
  before = true;

  start(o->name << '(');
  if (o->type->isClass() && !o->parens->isEmpty()) {
    ClassType* type = dynamic_cast<ClassType*>(o->type->strip());
    assert(type);
    middle("BI_NEW(" << type->name << ')');
    middle(o->parens); ///@todo Use genArgs()
  } else if (o->type->isArray()) {
    const ArrayType* type =
        dynamic_cast<const ArrayType*>(o->type.get());
    assert(type);
    middle("make_frame(" << type->brackets << ")");
    if (!o->parens->isEmpty()) {
      middle(", " << o->parens->strip()); ///@todo Use genArgs()
    }
  } else if (!o->value->isEmpty()) {
    middle(o->value);
  }
  middle(')');
}

void bi::CppConstructorGenerator::visit(const MemberFunction* o) {
  //
}

void bi::CppConstructorGenerator::visit(const ConversionOperator* o) {
  //
}

void bi::CppConstructorGenerator::visit(const AssignmentOperator* o) {
  //
}
