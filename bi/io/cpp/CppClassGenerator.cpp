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
  if (o->isAlias() || !o->braces->isEmpty()) {
    type = o;

    Gatherer<MemberFunction> memberFunctions;
    Gatherer<MemberFiber> memberFibers;
    Gatherer<MemberVariable> memberVariables;
    o->accept(&memberFunctions);
    o->accept(&memberFibers);
    o->accept(&memberVariables);

    /* start boilerplate */
    if (header) {
      if (!o->isAlias()) {
        genTemplateParams(o);
        start("class " << o->name);
        if (o->isGeneric() && o->isBound()) {
       	 genTemplateArgs(o);
        }
        if (o->isBound() && !o->base->isEmpty()) {
          middle(" : public ");
          ++inPointer;
          middle(o->base);
        }
        if (o->base->isEmpty()) {
          middle(" : public Any");
        }
        finish(" {");
        line("public:");
        in();
        if (o->isBound()) {
          start("using class_type = " << o->name);
          genTemplateArgs(o);
          finish(';');
          line("using this_type = class_type;");
          if (o->base->isEmpty()) {
            line("using super_type = Any;");
          } else {
            ++inPointer;
            line("using super_type = " << o->base << ';');
          }
          line("");
          line("using super_type::operator=;");
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
            line("using super_type::" << internalise(name) << ';');
          }
          line("");
        }
      }
    }

    if (!o->isAlias() && o->isBound()) {
      if (header) {
        out();
        line("protected:");
        in();
      }

      /* constructor */
      if (!header) {
        start("bi::type::" << o->name);
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
        if (!o->args->isEmpty()) {
          middle(o->args);
        }
        middle(')');
        ++inConstructor;
        for (auto o : memberVariables) {
          if (!o->value->isEmpty()) {
            finish(',');
            start(o->name << '(' << o->value << ')');
          } else if (o->type->isClass()) {
            finish(',');
            start(o->name << '(');
            ++inPointer;
            middle(o->type << "::create(" << o->args << ')');
            middle(')');
          } else if (o->type->isArray() && !o->brackets->isEmpty()) {
            finish(',');
            start(o->name << "(bi::make_frame(" << o->brackets << ')');
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

      /* copy constructor, destructor, assignment operator */
      if (header) {
        line(o->name << "(const " << o->name << "&) = default;");
        line("virtual ~" << o->name << "() = default;");
        line(o->name << "& operator=(const " << o->name << "&) = default;");
      }

      if (header) {
        out();
        line("public:");
        in();
      }

      /* standard functions */
      if (header) {
        line("STANDARD_CREATE_FUNCTION");
        line("STANDARD_EMPLACE_FUNCTION");
        line("STANDARD_CLONE_FUNCTION");
        line("STANDARD_DESTROY_FUNCTION");
      }

      /* freeze function */
      if (header) {
        start("virtual void ");
      } else {
        start("void bi::type::" << o->name);
        genTemplateArgs(o);
        middle("::");
      }
      middle("doFreeze()");
      if (header) {
        finish(';');
      } else {
        finish(" {");
        in();
        line("super_type::doFreeze();");
        for (auto o : memberVariables) {
          line("bi::freeze(" << o->name << ");");
        }
        out();
        line("}\n");
      }

      /* member variables and functions */
      *this << o->braces->strip();
    }

    /* end class */
    if (!o->isAlias() && header) {
      out();
      line("};\n");
    }

    /* C linkage function */
    if (!o->isGeneric() && o->params->isEmpty()) {
      if (header) {
        line("extern \"C\" " << o->name << "* make_" << o->name << "();");
      } else {
        line(
            "bi::type::" << o->name << "* bi::type::make_" << o->name << "() {");
        in();
        line("return bi::type::" << o->name << "::create();");
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
    line("SwapContext swap(context.get());");
    line("Shared<this_type> self(this);");

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
      line("SwapContext swap(context.get());");
      line("Shared<this_type> self(this);");
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
      line("SwapContext swap(context.get());");
      line("Shared<this_type> self(this);");
      CppBaseGenerator auxBase(base, level, header);
      auxBase << o->braces->strip();
      out();
      finish("}\n");
    }
  }
}
