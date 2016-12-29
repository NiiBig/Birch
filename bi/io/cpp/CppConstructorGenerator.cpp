/**
 * @file
 */
#include "bi/io/cpp/CppConstructorGenerator.hpp"
#include "bi/io/cpp/CppBaseGenerator.hpp"
#include "bi/visitor/Gatherer.hpp"

bi::CppConstructorGenerator::CppConstructorGenerator(std::ostream& base,
    const int level, const bool header) :
    indentable_ostream(base, level, header) {
  //
}

void bi::CppConstructorGenerator::visit(const ModelReference* o) {
  CppBaseGenerator aux(base, level, header);
  if (o->count() > 0) {
    middle("make_frame(");
    aux << o->brackets;
    middle(")*frame.lead");
  } else {
    middle("frame");
  }
}

void bi::CppConstructorGenerator::visit(const ModelParameter* o) {
  CppBaseGenerator aux(base, level, header);

  /* constructor with lead */
  if (header) {
    line("template<class Frame = EmptyFrame>");
    start(o->name->str() << '(');
    if (*o->parens) {
      aux << o->parens->strip();
      middle(", ");
    }
    middle("const Frame& frame = EmptyFrame()");
    middle(", const char* name = nullptr");
    middle(", const Group& group = Group()");
    middle(')');
    finish(" :");
    in();
    in();
    if (*o->base) {
      aux << o->base;
      finish(',');
    }
    start("group(childGroup(group, name))");

    Gatherer<VarDeclaration> gatherer;
    o->braces->accept(&gatherer);
    for (auto iter = gatherer.gathered.begin();
        iter != gatherer.gathered.end(); ++iter) {
      initialise(*iter);
    }

    out();
    out();
    finish(" {");
    in();
    for (auto iter = gatherer.gathered.begin();
        iter != gatherer.gathered.end(); ++iter) {
      assign(*iter);
    }
    out();
    line("}\n");
  }
}

void bi::CppConstructorGenerator::initialise(const VarDeclaration* o) {
  CppBaseGenerator aux(base, level, header);
  finish(',');
  start(o->param->name->str() << "(");
  if (*o->param->parens->strip() && o->param->type->count() == 0) {
    aux << o->param->parens->strip();
    middle(", ");
  }
  middle(o->param->type);
  middle(", \"" << o->param->name->str() << "\"");
  middle(", ");
  if (o->param->type->count() > 0) {
    middle("arrayGroup");
  } else {
    middle("childGroup");
  }
  middle("(this->group, \"" << o->param->name->str() << "\")");
  if (*o->param->parens->strip() && o->param->type->count() > 0) {
    middle(", ");
    aux << o->param->value->strip();
  }
  middle(')');
}

void bi::CppConstructorGenerator::assign(const VarDeclaration* o) {
  CppBaseGenerator aux(base, level, header);
  if (*o->param->value) {
    start(o->param->name->str() << " = ");
    aux << o->param->value;
    finish(';');
  }
}
