/**
 * @file
 */
#include "bi/io/cpp/CppClassGenerator.hpp"

#include "bi/io/cpp/CppMemberFiberGenerator.hpp"
#include "bi/primitive/encode.hpp"

bi::CppClassGenerator::CppClassGenerator(std::ostream& base, const int level,
    const bool header) :
    CppBaseGenerator(base, level, header),
    type(nullptr) {
  //
}

void bi::CppClassGenerator::visit(const Class* o) {
  if (!o->isAlias() && o->isBound() && !o->braces->isEmpty()) {
    type = o;

    Gatherer<MemberFunction> memberFunctions;
    Gatherer<MemberFiber> memberFibers;
    Gatherer<MemberVariable> memberVariables;
    o->accept(&memberFunctions);
    o->accept(&memberFibers);
    o->accept(&memberVariables);

    /* start boilerplate */
    if (header) {
      genTemplateParams(o);
      start("class " << o->name);
      if (o->isGeneric() && o->isBound()) {
       genTemplateArgs(o);
      }
      if (o->has(FINAL)) {
        middle(" final");
      }
      if (o->isBound() && !o->base->isEmpty()) {
        middle(" : public ");
        ++inPointer;
        middle(o->base);
      }
      if (o->base->isEmpty()) {
        middle(" : public libbirch::Any");
      }
      finish(" {");
      line("public:");
      in();
      if (o->isBound()) {
        start("using class_type_ = " << o->name);
        genTemplateArgs(o);
        finish(';');
        line("using this_type_ = class_type_;");
        if (o->base->isEmpty()) {
          line("using super_type_ = libbirch::Any;");
        } else {
          ++inPointer;
          line("using super_type_ = " << o->base << ';');
        }
        line("");
        line("using super_type_::operator=;");
        line("");

        /* using declarations for member functions and fibers in base classes
         * that are overridden */
        std::set<std::string> names;
        for (auto f : memberFunctions) {
          if (o->scope->override(f)) {
            names.insert(f->name->str());
          }
        }
        for (auto f : memberFibers) {
          if (o->scope->override(f)) {
            names.insert(f->name->str());
          }
        }
        for (auto name : names) {
          line("using super_type_::" << internalise(name) << ';');
        }
        line("");
      }
    }

    /* constructor */
    if (!header) {
      start("bi::type::" << o->name);
      genTemplateArgs(o);
      middle("::");
    } else {
      start("");
    }
    middle(o->name << "(libbirch::LazyLabel* label_");
    if (!o->params->isEmpty()) {
      CppBaseGenerator aux(base, level, header);
      aux << ", " << o->params;
    }
    middle(')');
    if (header) {
      finish(";\n");
    } else {
      finish(" :");
      in();
      in();
      start("super_type_(label_");
      if (!o->args->isEmpty()) {
        middle(", " << o->args);
      }
      middle(')');
      ++inConstructor;
      for (auto o : memberVariables) {
        if (!o->value->isEmpty()) {
          finish(',');
          start(o->name << '(' << o->value << ')');
        } else if (o->type->isClass()) {
          finish(',');
          ++inPointer;
          start(o->name << "(libbirch::make_object<" << o->type << ">(label_");
          if (!o->args->isEmpty()) {
            middle(", " << o->args);
          }
          middle("))");
        } else if (o->type->isArray() && !o->brackets->isEmpty()) {
          finish(',');
          start(o->name << "(libbirch::make_frame(" << o->brackets << ')');
          if (!o->args->isEmpty()) {
            middle(", " << o->args);
          }
          middle(')');
        }
      }
      --inConstructor;
      out();
      out();
      finish(" {");
      in();
      line("//");
      out();
      line("}\n");
    }

    /* deep copy constructor */
    if (!header) {
      start("bi::type::" << o->name);
      genTemplateArgs(o);
      middle("::");
    } else {
      start("");
    }
    middle(o->name << "(libbirch::Label* label_, const " << o->name << "& o_)");
    if (header) {
      finish(";\n");
    } else {
      finish(" :");
      in();
      in();
      start("super_type_(label_, o_)");
      for (auto o : memberVariables) {
        if (!o->type->isValue()) {
          finish(',');
          start(o->name << "(label_, o_." << o->name << ')');
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
      line("virtual ~" << o->name << "() = default;");
      line(o->name << "(const " << o->name << "&) = delete;");
      line(o->name << "& operator=(const " << o->name << "&) = delete;");
    }

    /* clone function */
    if (header) {
      line("virtual " << o->name << "* clone_(libbirch::Label* label_) const {");
      in();
      line("return new " << o->name << "(label_, *this);");
      out();
      line("}\n");
    }

    /* name function */
    if (header) {
      line("virtual const char* name_() const {");
      in();
      line("return \"" << o->name << "\";");
      out();
      line("}\n");
    }

    /* freeze function */
    line("#if ENABLE_LAZY_DEEP_CLONE");
    if (header) {
      start("virtual void ");
    } else {
      start("void bi::type::" << o->name);
      genTemplateArgs(o);
      middle("::");
    }
    middle("doFreeze_()");
    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();
      line("super_type_::doFreeze_();");
      for (auto o : memberVariables) {
        line("libbirch::freeze(" << o->name << ");");
      }
      out();
      line("}\n");
    }

    /* thaw function */
    if (header) {
      start("virtual void ");
    } else {
      start("void bi::type::" << o->name);
      genTemplateArgs(o);
      middle("::");
    }
    middle("doThaw_(libbirch::LazyLabel* label_)");
    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();
      line("super_type_::doThaw_(label_);");
      for (auto o : memberVariables) {
        line("libbirch::thaw(" << o->name << ", label_);");
      }
      out();
      line("}\n");
    }

    /* finish function */
    if (header) {
      start("virtual void ");
    } else {
      start("void bi::type::" << o->name);
      genTemplateArgs(o);
      middle("::");
    }
    middle("doFinish_()");
    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();
      line("super_type_::doFinish_();");
      for (auto o : memberVariables) {
        line("libbirch::finish(" << o->name << ");");
      }
      out();
      line("}");
    }
    line("#endif\n");

    /* setters for member variables */
    if (header) {
      Gatherer<MemberVariable> memberVars;
      o->accept(&memberVars);
      for (auto var : memberVars) {
        line("template<class T_>");
        line("auto& set_" << var->name << "_(T_&& o_) {");
        in();
        line("libbirch_swap_context_");
        line("return " << var->name << " = " << "o_;");
        out();
        line("}\n");

        if (var->type->isArray()) {
          line("template<class F_, class T_>");
          line("auto set_" << var->name << "_(const F_& frame_, T_&& o_) {");
          in();
          line("libbirch_swap_context_");
          line("return " << var->name << "(frame_) = " << "o_;");
          out();
          line("}\n");
        }
      }
    }

    /* member variables and functions */
    *this << o->braces->strip();

    /* end class */
    if (header) {
      out();
      line("};\n");
    }

    /* C linkage function */
    if (!o->isGeneric() && o->params->isEmpty()) {
      if (header) {
        start("extern \"C\" bi::type::" << o->name << "* ");
        finish("make_" << o->name << "_();");
      } else {
        start("bi::type::" << o->name << "* ");
        finish("bi::type::make_" << o->name << "_() {");
        in();
        line("return new bi::type::" << o->name << "();");
        out();
        line("}");
      }
      line("");
    }
  }
}

void bi::CppClassGenerator::visit(const MemberVariable* o) {
  if (header) {
    line(o->type << ' ' << o->name << ';');
  }
}

void bi::CppClassGenerator::visit(const MemberFunction* o) {
  if (header) {
    start("virtual ");
  } else {
    start("");
  }
  middle(o->returnType << ' ');
  if (!header) {
    middle("bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::");
  }
  middle(internalise(o->name->str()) << '(' << o->params << ')');
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    genTraceFunction(o->name->str(), o->loc);

	  /* declare self and swap in context if necessary */
    Gatherer<Member> members;
    Gatherer<Raw> raws;
    Gatherer<This> selfs;
    Gatherer<Super> supers;
    o->accept(&members);
    o->accept(&raws);
    o->accept(&selfs);
    o->accept(&supers);
    if (members.size() + raws.size() + selfs.size() + supers.size() > 0) {
      line("libbirch_swap_context_");
      line("libbirch_declare_self_");
    }

    /* body */
    CppBaseGenerator auxBase(base, level, header);
    auxBase << o->braces->strip();

    out();
    finish("}\n");
  }
}

void bi::CppClassGenerator::visit(const MemberFiber* o) {
  CppMemberFiberGenerator auxMemberFiber(type, base, level, header);
  auxMemberFiber << o;
}

void bi::CppClassGenerator::visit(const AssignmentOperator* o) {
  if (!o->braces->isEmpty()) {
    if (header) {
      start("virtual ");
    } else {
      start("bi::type::");
    }
    middle(type->name);
    genTemplateArgs(type);
    middle("& ");
    if (!header) {
      middle("bi::type::" << type->name);
      genTemplateArgs(type);
      middle("::");
    }
    middle("operator=(" << o->single << ')');
    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();
      genTraceFunction("<assignment>", o->loc);
      line("libbirch_swap_context_");
      line("libbirch_declare_self_");
      CppBaseGenerator auxBase(base, level, header);
      auxBase << o->braces->strip();
      line("return *this;");
      out();
      finish("}\n");
    }
  }
}

void bi::CppClassGenerator::visit(const ConversionOperator* o) {
  if (!o->braces->isEmpty()) {
    if (!header) {
      start("bi::type::" << type->name);
      genTemplateArgs(type);
      middle("::");
    } else {
      start("virtual ");
    }
    middle("operator " << o->returnType << "()");
    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();
      genTraceFunction("<conversion>", o->loc);
      line("libbirch_swap_context_");
      line("libbirch_declare_self_");
      CppBaseGenerator auxBase(base, level, header);
      auxBase << o->braces->strip();
      out();
      finish("}\n");
    }
  }
}
