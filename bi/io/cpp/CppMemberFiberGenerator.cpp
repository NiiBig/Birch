/**
 * @file
 */
#include "bi/io/cpp/CppMemberFiberGenerator.hpp"

#include "bi/visitor/Gatherer.hpp"
#include "bi/io/bih_ostream.hpp"
#include "bi/primitive/encode.hpp"

bi::CppMemberFiberGenerator::CppMemberFiberGenerator(const Class* type,
    std::ostream& base, const int level, const bool header) :
    CppFiberGenerator(base, level, header),
    type(type) {
  //
}

void bi::CppMemberFiberGenerator::visit(const MemberFiber* o) {
  fiberType = dynamic_cast<const FiberType*>(o->returnType);
  assert(fiberType);

  /* generate a unique name (within this scope) for the state of the fiber */
  std::stringstream base;
  bih_ostream buf(base);
  buf << o->params;
  std::string baseName = internalise(o->name->str()) + encode32(base.str());
  std::string stateName = baseName + "_FiberState_";

  /* gather important objects */
  o->params->accept(&params);
  o->braces->accept(&locals);
  o->braces->accept(&yields);

  /* supporting class for state */
  if (header) {
    start("class " << stateName << " final : ");
    finish("public libbirch::FiberState<" << fiberType->yieldType << "> {");
    line("public:");
    in();
    line("using class_type_ = " << stateName << ';');
    line("using super_type_ = libbirch::FiberState<" << fiberType->yieldType << ">;\n");
    start("libbirch::LazySharedPtr<bi::type::" << type->name);
    genTemplateArgs(type);
    finish("> self;");
    for (auto o : params) {
      line(o->type << ' ' << o->name << ';');
    }
    for (auto o : locals) {
      if (o->has(IN_FIBER)) {
        start(o->type << ' ');
        finish(getName(o->name->str(), o->number) << ';');
      }
    }
  }

  /* constructor */
  start("");
  if (!header) {
    genSourceLine(o->loc);
    middle("bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::" << stateName << "::");
  }
  middle(stateName << "(const libbirch::LazySharedPtr<");
  middle(type->name);
  genTemplateArgs(type);
  middle(">& self");
  if (!o->params->isEmpty()) {
    middle(", " << o->params);
  }
  middle(')');
  if (header) {
    finish(';');
  } else {
    finish(" :");
    in();
    in();
    genSourceLine(o->loc);
    start("super_type_(" << (yields.size() + 1) << ')');
    finish(',');
    genSourceLine(o->loc);
    start("self(self)");
    for (auto param : params) {
      finish(',');
      genSourceLine(param->loc);
      start(param->name << '(' << param->name << ')');
    }
    finish(" {");
    out();
    line("//");
    out();
    line("}\n");
  }

  /* deep copy constructor */
  if (!header) {
    genSourceLine(o->loc);
    middle("bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::" << stateName << "::");
  } else {
    start("");
  }
  middle(stateName << "(libbirch::Label* label, const " << stateName << "& o)");
  if (header) {
    finish(";\n");
  } else {
    finish(" :");
    in();
    in();
    genSourceLine(o->loc);
    start("super_type_(label, o)");
    finish(',');
    genSourceLine(o->loc);
    start("self(label, o.self)");
    for (auto o : params) {
      finish(',');
      genSourceLine(o->loc);
      if (o->type->isValue()) {
        start(o->name << "(o." << o->name << ')');
      } else {
        start(o->name << "(label, o." << o->name << ')');
      }
    }
    for (auto o : locals) {
      if (o->has(IN_FIBER)) {
        auto name = getName(o->name->str(), o->number);
        finish(',');
        genSourceLine(o->loc);
        if (o->type->isValue()) {
          start(name << "(o." << name << ')');
        } else {
          start(name << "(context, label, o." << name << ')');
        }
      }
    }
    out();
    out();
    finish(" {");
    in();
    line("//");
    out();
    line("}\n");
  }

  /* copy constructor, destructor, assignment operator */
  if (header) {
    line("virtual ~" << stateName << "() = default;");
    line(stateName << "(const " << stateName << "&) = delete;");
    line(stateName << "& operator=(const " << stateName << "&) = delete;");
  }

  /* clone function */
  if (header) {
    line("virtual " << stateName << "* clone_(libbirch::Label* label) const;");
  } else {
    genSourceLine(o->loc);
    start("bi::type::" << stateName << "* bi::type::" << stateName);
    finish("::clone_(libbirch::Label* label) const {");
    in();
    genSourceLine(o->loc);
    line("return new class_type_(label, *this);");
    genSourceLine(o->loc);
    out();
    line("}\n");
  }

  /* name function */
  if (header) {
    line("virtual const char* getClassName() const;");
  } else {
    genSourceLine(o->loc);
    start("const char* bi::type::" << stateName);
    finish("::getClassName() const {");
    in();
    genSourceLine(o->loc);
    line("return \"" << stateName << "\";");
    genSourceLine(o->loc);
    out();
    line("}\n");
  }

  /* freeze function */
  if (header) {
    start("virtual void ");
  } else {
    genSourceLine(o->loc);
    start("void bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::" << stateName << "::");
  }
  middle("doFreeze_()");
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    genSourceLine(o->loc);
    line("super_type_::doFreeze_();");
    genSourceLine(o->loc);
    line("freeze(self);");
    genSourceLine(o->loc);
    line("freeze(value_);");
    for (auto o : params) {
      genSourceLine(o->loc);
      line("freeze(" << o->name << ");");
    }
    for (auto o : locals) {
      genSourceLine(o->loc);
      line("freeze(" << getName(o->name->str(), o->number) << ");");
    }
    genSourceLine(o->loc);
    out();
    line("}\n");
  }

  /* thaw function */
  if (header) {
    start("virtual void ");
  } else {
    genSourceLine(o->loc);
    start("void bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::" << stateName << "::");
  }
  middle("doThaw_(libbirch::Label* label_)");
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    genSourceLine(o->loc);
    line("super_type_::doThaw_(label_);");
    genSourceLine(o->loc);
    line("thaw(self, label_);");
    genSourceLine(o->loc);
    line("thaw(value_, label_);");
    for (auto o : params) {
      genSourceLine(o->loc);
      line("thaw(" << o->name << ", label_);");
    }
    for (auto o : locals) {
      genSourceLine(o->loc);
      line("thaw(" << getName(o->name->str(), o->number) << ", label_);");
    }
    genSourceLine(o->loc);
    out();
    line("}\n");
  }

  /* finish function */
  if (header) {
    start("virtual void ");
  } else {
    genSourceLine(o->loc);
    start("void bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::" << stateName << "::");
  }
  middle("doFinish_()");
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    genSourceLine(o->loc);
    line("super_type_::doFinish_();");
    genSourceLine(o->loc);
    line("finish(self);");
    genSourceLine(o->loc);
    line("finish(value_);");
    for (auto o : params) {
      genSourceLine(o->loc);
      line("finish(" << o->name << ");");
    }
    for (auto o : locals) {
      genSourceLine(o->loc);
      line("finish(" << getName(o->name->str(), o->number) << ");");
    }
    genSourceLine(o->loc);
    out();
    line("}");
  }

  /* query function */
  if (header) {
    start("virtual ");
  } else {
    genSourceLine(o->loc);
    start("");
  }
  middle("bool ");
  if (!header) {
    middle("bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::" << stateName << "::");
  }
  middle("query()");
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    genTraceFunction(o->name->str(), o->loc);
    line("libbirch_declare_local_");
    genSourceLine(o->loc);
    line("switch (local->point_) {");
    in();
    for (int s = 0; s <= yields.size(); ++s) {
      genSourceLine(o->loc);
      line("case " << s << ": goto POINT" << s << "_;");
    }
    genSourceLine(o->loc);
    line("default: goto END_;");
    genSourceLine(o->loc);
    out();
    line('}');
    genSourceLine(o->loc);
    line("POINT0_:");
    ++point;

    *this << o->braces->strip();

    genSourceLine(o->loc);
    line("END_:");
    genSourceLine(o->loc);
    line("local->point_ = " << (yields.size() + 1) << ';');
    genSourceLine(o->loc);
    line("return false;");
    genSourceLine(o->loc);
    out();
    finish("}\n");
  }
  if (header) {
    out();
    line("};\n");
  }

  /* initialisation function */
  if (header) {
    start("virtual ");
  } else {
    genSourceLine(o->loc);
    start("");
  }
  middle(fiberType << ' ');
  if (!header) {
    middle("bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::");
  }
  middle(o->name << '(' << o->params << ')');
  if (header) {
    if (o->has(FINAL)) {
      middle(" final");
    } else if (o->has(ABSTRACT)) {
      middle(" = 0");
    }
    finish(';');
  } else {
    finish(" {");
    in();
    genSourceLine(o->loc);
    line("libbirch_declare_self_");
    genSourceLine(o->loc);
    start("return libbirch::make_fiber<" << stateName << ">(self");
    for (auto param: params) {
      middle(", " << param->name);
    }
    finish(");");
    genSourceLine(o->loc);
    out();
    finish("}\n");
  }
}

void bi::CppMemberFiberGenerator::visit(const This* o) {
  middle("local->self");
}

void bi::CppMemberFiberGenerator::visit(const Super* o) {
  middle("local->self");
}
