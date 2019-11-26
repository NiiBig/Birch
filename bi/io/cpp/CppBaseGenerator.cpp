/**
 * @file
 */
#include "bi/io/cpp/CppBaseGenerator.hpp"

#include "bi/io/cpp/CppClassGenerator.hpp"
#include "bi/io/cpp/CppFiberGenerator.hpp"
#include "bi/io/bih_ostream.hpp"
#include "bi/primitive/encode.hpp"

#include "boost/algorithm/string.hpp"

bi::CppBaseGenerator::CppBaseGenerator(std::ostream& base, const int level,
    const bool header) :
    indentable_ostream(base, level),
    header(header),
    inAssign(0),
    inConstructor(0),
    inLambda(0),
    inSequence(0) {
  //
}

void bi::CppBaseGenerator::visit(const Name* o) {
  middle(internalise(o->str()));
}

void bi::CppBaseGenerator::visit(const ExpressionList* o) {
  middle(o->head);
  if (o->tail) {
    middle(", " << o->tail);
  }
}

void bi::CppBaseGenerator::visit(const Literal<bool>* o) {
  middle(o->str);
}

void bi::CppBaseGenerator::visit(const Literal<int64_t>* o) {
  middle("std::int64_t(" << o->str << ')');
}

void bi::CppBaseGenerator::visit(const Literal<double>* o) {
  middle(o->str);
}

void bi::CppBaseGenerator::visit(const Literal<const char*>* o) {
  middle("std::string(" << o->str << ')');
}

void bi::CppBaseGenerator::visit(const Parentheses* o) {
  if (o->single->type->isList()) {
    if (inAssign) {
      middle("libbirch::tie(");
    } else {
      middle("libbirch::make_tuple(");
      if (!o->type->isValue()) {
        middle("context_, ");
      }
    }
  } else {
    middle('(');
  }
  middle(o->single << ')');
}

void bi::CppBaseGenerator::visit(const Sequence* o) {
  if (o->single->isEmpty()) {
    middle("libbirch::nil");
  } else {
    if (inSequence == 0) {
      auto type = dynamic_cast<ArrayType*>(o->type);
      assert(type);
      middle("libbirch::make_array<" << type->element() << ">(");
      if (!o->type->isValue()) {
        middle("context_, ");
      }
      middle("libbirch::make_shape(");
      auto seq = o;
      do {
        if (seq != o) {
          middle(", ");
        }
        middle(seq->single->width());
        seq = dynamic_cast<const Sequence*>(*seq->single->begin());
      } while (seq);
      middle("), std::initializer_list<" << type->element() << ">({ ");
    }
    ++inSequence;
    middle(o->single);
    --inSequence;
    if (inSequence == 0) {
      middle(" }))");
    }
  }
}

void bi::CppBaseGenerator::visit(const Cast* o) {
  if (o->returnType->isClass()) {
    middle("libbirch::dynamic_pointer_cast<" << o->returnType << '>');
    middle("(context_, " << o->single << ')');
  } else {
    middle("libbirch::check_cast<" << o->returnType << '>');
    middle('(' << o->single << ')');
  }
}

void bi::CppBaseGenerator::visit(const Call<Unknown>* o) {
  assert(false);
}

void bi::CppBaseGenerator::visit(const Call<Parameter>* o) {
  middle(o->single);
  middle('(');
  auto type = dynamic_cast<const FunctionType*>(o->target->type);
  assert(type);
  genArgs(o->args, type->params);
  middle(')');
}

void bi::CppBaseGenerator::visit(const Call<LocalVariable>* o) {
  middle(o->single);
  middle('(');
  auto type = dynamic_cast<const FunctionType*>(o->target->type);
  assert(type);
  genArgs(o->args, type->params);
  middle(')');
}

void bi::CppBaseGenerator::visit(const Call<MemberVariable>* o) {
  middle(o->single);
  middle('(');
  auto type = dynamic_cast<const FunctionType*>(o->target->type);
  assert(type);
  genArgs(o->args, type->params);
  middle(')');
}

void bi::CppBaseGenerator::visit(const Call<GlobalVariable>* o) {
  middle(o->single);
  middle('(');
  auto type = dynamic_cast<const FunctionType*>(o->target->type);
  assert(type);
  genArgs(o->args, type->params);
  middle(')');
}

void bi::CppBaseGenerator::visit(const Call<Function>* o) {
  middle(o->single);
  middle('(');
  if (!o->target->isValue()) {
    middle("context_");
    if (!o->args->isEmpty()) {
      middle(", ");
    }
  }
  genArgs(o->args, o->target->params->type);
  middle(')');
}

void bi::CppBaseGenerator::visit(const Call<MemberFunction>* o) {
  middle(o->single);
  middle('(');
  genArgs(o->args, o->target->params->type);
  middle(')');
}

void bi::CppBaseGenerator::visit(const Call<Fiber>* o) {
  middle(o->single);
  middle('(');
  if (!o->target->isValue()) {
    middle("context_");
    if (!o->args->isEmpty()) {
      middle(", ");
    }
  }
  genArgs(o->args, o->target->params->type);
  middle(')');
}

void bi::CppBaseGenerator::visit(const Call<MemberFiber>* o) {
  middle(o->single);
  middle('(');
  genArgs(o->args, o->target->params->type);
  middle(')');
}

void bi::CppBaseGenerator::visit(const Call<BinaryOperator>* o) {
  auto op = dynamic_cast<OverloadedIdentifier<BinaryOperator>*>(o->single);
  assert(op);
  if (isTranslatable(op->name->str()) && o->target->isValue()) {
    /* use corresponding C++ operator */
    genLeftArg(o);
    middle(' ' << op->name->str() << ' ');
    genRightArg(o);
  } else {
    /* use function */
    middle(o->single << '(');
    if (!o->target->isValue()) {
      middle("context_, ");
    }
    genLeftArg(o);
    middle(", ");
    genRightArg(o);
    middle(')');
  }
}

void bi::CppBaseGenerator::visit(const Call<UnaryOperator>* o) {
  auto op = dynamic_cast<OverloadedIdentifier<UnaryOperator>*>(o->single);
  assert(op);
  if (isTranslatable(op->name->str()) && o->target->isValue()) {
    /* use corresponding C++ operator */
    middle(op->name->str());
    genSingleArg(o);
  } else {
    /* use function */
    middle(o->single << '(');
    if (!o->target->isValue()) {
      middle("context_, ");
    }
    genSingleArg(o);
    middle(')');
  }
}

void bi::CppBaseGenerator::visit(const Assign* o) {
  /* determine whether this is setting a member variable outside of the
   * current class */
  auto member = dynamic_cast<const Member*>(o->left);
  auto slice = dynamic_cast<const Slice*>(o->left);
  const This* self = nullptr;
  const Super* super = nullptr;
  const Identifier<MemberVariable>* var = nullptr;

  if (slice) {
    member = dynamic_cast<const Member*>(slice->single);
  }
  if (member) {
    self = dynamic_cast<decltype(self)>(member->left);
    super = dynamic_cast<decltype(super)>(member->left);
    var = dynamic_cast<decltype(var)>(member->right);
  }

  if (var && !self && !super) {
    /* this assignment is setting a member variable outside of the current
     * class, use the setter member function of the class to ensure the
     * context is set correctly */
    ++inAssign;
    middle(member->left << "->set_" << var->name << "_(");
    --inAssign;
    if (slice) {
      middle("libbirch::make_slice(" << slice->brackets << "), ");
    }
    middle(o->right << ')');
  } else {
    if (o->left->type->isValue()) {
      ++inAssign;
      middle(o->left << " = ");
      --inAssign;
      genArg(o->right, o->left->type);
      //middle(o->right);
    } else {
      ++inAssign;
      middle(o->left << ".assign(context_, ");
      --inAssign;
      genArg(o->right, o->left->type);
      //middle(o->right);
      middle(')');
    }
  }
}

void bi::CppBaseGenerator::visit(const Slice* o) {
  middle(o->single << '.');
  if (inAssign) {
    middle("get");
  } else {
    middle("pull");
  }
  middle("(libbirch::make_slice(" << o->brackets << "))");
}

void bi::CppBaseGenerator::visit(const Query* o) {
  middle(o->single << ".query()");
}

void bi::CppBaseGenerator::visit(const Get* o) {
  middle(o->single);
  if (!o->single->type->isWeak()) {
    // ^ for a weak pointer, the ordinary dereference suffices
    middle(".get()");
  }
}

void bi::CppBaseGenerator::visit(const LambdaFunction* o) {
  finish("[=](" << o->params << ") {");
  in();
  ++inLambda;
  *this << o->braces->strip();
  --inLambda;
  out();
  start("}");
}

void bi::CppBaseGenerator::visit(const Span* o) {
  if (o->single->isEmpty()) {
    middle('0');
  } else {
    middle(o->single);
  }
}

void bi::CppBaseGenerator::visit(const Index* o) {
  middle(o->single << " - 1");
}

void bi::CppBaseGenerator::visit(const Range* o) {
  middle("libbirch::make_range(" << o->left << " - 1, " << o->right << " - 1)");
}

void bi::CppBaseGenerator::visit(const Member* o) {
  auto leftThis = dynamic_cast<const This*>(o->left);
  auto leftSuper = dynamic_cast<const Super*>(o->left);
  if (inConstructor && (leftThis || leftSuper)) {
    if (leftThis) {
      middle("this->");
    } else if (leftSuper) {
      middle("super_type_::");
    }
    middle(o->right);
  } else {
    auto rightVar = dynamic_cast<const Identifier<MemberVariable>*>(o->right);
    middle(o->left);
    if (!inAssign && rightVar && rightVar->type->isValue()) {
      /* optimization: just reading a value, so no need to copy-on-write the
       * owning object */
      middle(".pull()");
    }
    middle("->");

    /* explicitly refer to the super class if necessary */
    auto leftSuper = dynamic_cast<const Super*>(o->left);
    if (leftSuper) {
      middle("super_type_::");
    }
    middle(o->right);
  }
}

void bi::CppBaseGenerator::visit(const This* o) {
  middle("self");
}

void bi::CppBaseGenerator::visit(const Super* o) {
  middle("self");
}

void bi::CppBaseGenerator::visit(const Global* o) {
  middle("::" << o->single);
}

void bi::CppBaseGenerator::visit(const Nil* o) {
  middle("libbirch::nil");
}

void bi::CppBaseGenerator::visit(const Parameter* o) {
  middle("const " << o->type);
  if (o->type->isArray() || o->type->isClass()) {
    middle('&');
  }
  middle(' ' << o->name);
  if (!o->value->isEmpty()) {
    middle(" = " << o->value);
  }
}

void bi::CppBaseGenerator::visit(const Identifier<Unknown>* o) {
  assert(false);  // should have been resolved in Resolver
}

void bi::CppBaseGenerator::visit(const Identifier<Parameter>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const Identifier<GlobalVariable>* o) {
  middle("bi::" << o->name << "()");
}

void bi::CppBaseGenerator::visit(const Identifier<MemberVariable>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const Identifier<LocalVariable>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const Identifier<ForVariable>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const Identifier<ParallelVariable>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const OverloadedIdentifier<Function>* o) {
  auto name = internalise(o->name->str());
  if (o->overload->isInstantiation()) {
    std::stringstream base;
    bih_ostream buf(base);
    buf << o->overload->typeParams << '(' << o->overload->params->type << ')';
    name += "_" + encode32(base.str()) + "_";
  }
  middle("bi::" << name);
}

void bi::CppBaseGenerator::visit(const OverloadedIdentifier<Fiber>* o) {
  auto name = internalise(o->name->str());
  if (o->overload->isInstantiation()) {
    std::stringstream base;
    bih_ostream buf(base);
    buf << o->overload->typeParams << '(' << o->overload->params->type << ')';
    name += "_" + encode32(base.str()) + "_";
  }
  middle("bi::" << name);
}

void bi::CppBaseGenerator::visit(
    const OverloadedIdentifier<MemberFunction>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const OverloadedIdentifier<MemberFiber>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(
    const OverloadedIdentifier<BinaryOperator>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(
    const OverloadedIdentifier<UnaryOperator>* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const File* o) {
  *this << o->root;
}

void bi::CppBaseGenerator::visit(const GlobalVariable* o) {
  /* C++ does not guarantee static initialization order across compilation
   * units. Global variables are therefore used through accessor functions
   * that initialize their values on first use. */
  start(o->type << "& ");
  if (!header) {
    middle("bi::");
  }
  middle(o->name << "()");
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    genTraceLine(o->loc);
    line("auto context_ [[maybe_unused]] = libbirch::rootContext;");
    genTraceLine(o->loc);
    start("static " << o->type << " result");
    genInit(o);
    finish(';');
    genTraceLine(o->loc);
    line("return result;");
    out();
    line("}\n");
  }
}

void bi::CppBaseGenerator::visit(const MemberVariable* o) {
  assert(false);  // should be in CppClassGenerator
}

void bi::CppBaseGenerator::visit(const LocalVariable* o) {
  genTraceLine(o->loc);
  middle(o->type << ' ' << o->name);
  genInit(o);
  finish(';');
}

void bi::CppBaseGenerator::visit(const ForVariable* o) {
  /* no need to include the type here, used only as though an identifier by
   * visit(const For*) */;
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const ParallelVariable* o) {
  /* no need to include the type here, used only as though an identifier by
   * visit(const Parallel*) */;
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const Function* o) {
  if (!o->braces->isEmpty() && o->isBound()) {
    auto name = internalise(o->name->str());
    if (o->isInstantiation()) {
      std::stringstream base;
      bih_ostream buf(base);
      buf << o->typeParams << '(' << o->params->type << ')';
      name += "_" + encode32(base.str()) + "_";
    }
    if (!header) {
      genSourceLine(o->loc);
    }
    start(o->returnType << ' ');
    if (!header) {
      middle("bi::");
    }
    middle(name << '(');
    if (!o->isValue()) {
      middle("libbirch::Label* context_");
      if (!o->params->isEmpty()) {
        middle(", ");
      }
    }
    middle(o->params << ')');
    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();
      for (auto iter = o->typeParams->begin(); iter != o->typeParams->end();
          ++iter) {
        auto param = dynamic_cast<const Generic*>(*iter);
        assert(param);
        genTraceLine(o->loc);
        line("using " << param->name << " [[maybe_unused]] = " << param->type << ';');
      }
      genTraceFunction(o->name->str(), o->loc);
      CppBaseGenerator aux(base, level, false);
      *this << o->braces->strip();
      out();
      line("}\n");
    }
  }
  for (auto instantiation : o->instantiations) {
    *this << instantiation;
  }
}

void bi::CppBaseGenerator::visit(const Fiber* o) {
  if (header || o->isBound()) {
    CppFiberGenerator auxFiber(base, level, header);
    auxFiber << o;
  }
  for (auto instantiation : o->instantiations) {
    *this << instantiation;
  }
}

void bi::CppBaseGenerator::visit(const MemberFunction* o) {
  assert(false);  // should be in CppClassGenerator
}

void bi::CppBaseGenerator::visit(const MemberFiber* o) {
  assert(false);  // should be in CppClassGenerator
}

void bi::CppBaseGenerator::visit(const Program* o) {
  if (header) {
    line("extern \"C\" int " << o->name << "(int, char**);");
  } else {
    genSourceLine(o->loc);
    line("int bi::" << o->name << "(int argc_, char** argv_) {");
    in();
    genTraceFunction(o->name->str(), o->loc);

    /* initial context */
    genSourceLine(o->loc);
    line("auto context_ [[maybe_unused]] = libbirch::rootContext;");

    /* handle program options */
    if (o->params->width() > 0) {
      /* option variables */
      for (auto iter = o->params->begin(); iter != o->params->end(); ++iter) {
        auto param = dynamic_cast<const Parameter*>(*iter);
        assert(param);
        genSourceLine(o->loc);
        start(param->type << ' ' << param->name);
        if (!param->value->isEmpty()) {
          middle(" = " << param->value);
        } else if (param->type->isClass()) {
          middle(" = libbirch::make_pointer<" << param->type << ">(context_)");
        }
        finish(';');
      }
      line("");

      /* option flags */
      line("enum {");
      in();
      for (auto param : *o->params) {
        auto name = dynamic_cast<const Parameter*>(param)->name;
        std::string flag = internalise(name->str()) + "FLAG_";
        line(flag << ',');
      }
      out();
      line("};");

      /* long options */
      genSourceLine(o->loc);
      line("int c_, option_index_;");
      genSourceLine(o->loc);
      line("option long_options_[] = {");
      in();
      for (auto param : *o->params) {
        auto name = dynamic_cast<const Parameter*>(param)->name;
        std::string flag = internalise(name->str()) + "FLAG_";

        std::string option = name->str();
        boost::replace_all(option, "_", "-");

        genSourceLine(o->loc);
        start("{\"");
        middle(option << "\", required_argument, 0, " << flag);
        finish(" },");
      }
      genSourceLine(o->loc);
      line("{0, 0, 0, 0}");
      out();
      genSourceLine(o->loc);
      line("};");

      /* short options */
      genSourceLine(o->loc);
      line("const char* short_options_ = \":\";");

      /* handle error reporting ourselves (worth commenting this out if
       * debugging issues with command-line parsing) */
      genSourceLine(o->loc);
      line("::opterr = 0;");

      /* read in options with getopt_long */
      genSourceLine(o->loc);
      start("c_ = ::getopt_long_only(argc_, argv_, short_options_, ");
      finish("long_options_, &option_index_);");
      genSourceLine(o->loc);
      line("while (c_ != -1) {");
      in();
      genSourceLine(o->loc);
      line("switch (c_) {");
      in();

      for (auto param : *o->params) {
        auto name = dynamic_cast<const Parameter*>(param)->name;
        std::string flag = internalise(name->str()) + "FLAG_";

        genSourceLine(param->loc);
        line("case " << flag << ':');
        in();
        genSourceLine(param->loc);
        line("libbirch_error_msg_(::optarg, \"option --\" << long_options_[::optopt].name << \" requires a value.\");");
        genSourceLine(param->loc);
        if (param->type->unwrap()->isBasic()) {
          auto type = dynamic_cast<Named*>(param->type->unwrap());
          assert(type);
          start(name << " = bi::" << type->name);
          finish("(std::string(::optarg));");
        } else {
          line(name << " = std::string(::optarg);");
        }
        line("break;");
        out();
      }

      genSourceLine(o->loc);
      line("case '?':");
      in();
      genSourceLine(o->loc);
      line("libbirch_error_msg_(false, \"option \" << argv_[::optind - 1] << \" unrecognized.\");");
      out();

      genSourceLine(o->loc);
      line("case ':':");
      in();
      genSourceLine(o->loc);
      line("libbirch_error_msg_(false, \"option --\" << long_options_[::optopt].name << \" requires a value.\");");
      out();

      genSourceLine(o->loc);
      line("default:");
      in();
      genSourceLine(o->loc);
      line("libbirch_error_msg_(false, std::string(\"unknown error parsing command-line options.\"));");
      out();

      out();
      line('}');
      genSourceLine(o->loc);
      start("c_ = ::getopt_long_only(argc_, argv_, short_options_, ");
      finish("long_options_, &option_index_);");
      out();
      line("}\n");
    }

    /* seed random number generator with random entropy */
    genTraceLine(o->loc);
    line("bi::seed();\n");

    /* body of program */
    if (!o->braces->isEmpty()) {
      CppBaseGenerator aux(base, level, header);
      aux << o->braces->strip();
    }

    genTraceLine(o->loc);
    line("return 0;");
    out();
    line("}\n");
  }
}

void bi::CppBaseGenerator::visit(const BinaryOperator* o) {
  if (!o->braces->isEmpty()) {
    if (!header) {
      genSourceLine(o->loc);
    }
    start(o->returnType << ' ');
    if (!header) {
      middle("bi::");
    }
    if (isTranslatable(o->name->str()) && o->isValue()) {
      middle("operator" << o->name->str() << '(');
    } else {
      middle(o->name << "(libbirch::Label* context_, ");
    }
    middle(o->params->getLeft() << ", " << o->params->getRight());
    middle(')');
    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();
      genTraceFunction(o->name->str(), o->loc);
      CppBaseGenerator aux(base, level, false);
      aux << o->braces->strip();
      out();
      finish("}\n");
    }
  }
}

void bi::CppBaseGenerator::visit(const UnaryOperator* o) {
  if (!o->braces->isEmpty()) {
    if (!header) {
      genSourceLine(o->loc);
    }
    start(o->returnType << ' ');
    if (!header) {
      middle("bi::");
    }
    if (isTranslatable(o->name->str()) && o->isValue()) {
      middle("operator" << o->name->str() << '(');
    } else {
      middle(o->name << "(libbirch::Label* context_, ");
    }
    middle(o->params << ')');
    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();
      genTraceFunction(o->name->str(), o->loc);
      CppBaseGenerator aux(base, level, false);
      aux << o->braces->strip();
      out();
      finish("}\n");
    }
  }
}

void bi::CppBaseGenerator::visit(const AssignmentOperator* o) {
  assert(false);  // should be in CppClassGenerator
}

void bi::CppBaseGenerator::visit(const ConversionOperator* o) {
  assert(false);  // should be in CppClassGenerator
}

void bi::CppBaseGenerator::visit(const Basic* o) {
  //
}

void bi::CppBaseGenerator::visit(const Class* o) {
  CppClassGenerator auxClass(base, level, header);
  auxClass << o;
}

void bi::CppBaseGenerator::visit(const Generic* o) {
  if (o->type->isEmpty()) {
    middle(o->name);
  } else {
    middle(o->type);
  }
}

void bi::CppBaseGenerator::visit(const Assume* o) {
  assert(false);  // should have been replaced by Resolver
}

void bi::CppBaseGenerator::visit(const ExpressionStatement* o) {
  genTraceLine(o->loc);
  line(o->single << ';');
}

void bi::CppBaseGenerator::visit(const If* o) {
  genTraceLine(o->loc);
  start("if (");
  auto cond = o->cond->strip();
  if (cond->type->isClass()) {
    middle("*(");
  }
  middle(cond);
  if (cond->type->isClass()) {
    middle(')');
  }
  finish(") {");
  in();
  *this << o->braces->strip();
  out();
  if (!o->falseBraces->isEmpty()) {
    line("} else {");
    in();
    *this << o->falseBraces->strip();
    out();
  }
  line("}");
}

void bi::CppBaseGenerator::visit(const For* o) {
  genTraceLine(o->loc);
  start("for (auto " << o->index << " = " << o->from << "; ");
  middle(o->index << " <= " << o->to << "; ");
  finish("++" << o->index << ") {");
  in();
  *this << o->braces->strip();
  out();
  line("}");
}

void bi::CppBaseGenerator::visit(const Parallel* o) {
  genTraceLine(o->loc);
  line("#pragma omp parallel");
  line("{");
  in();
  genTraceFunction("<parallel for>", o->loc);
  start("#pragma omp for schedule(");
  if (o->has(DYNAMIC)) {
    middle("guided");
  } else {
    middle("static");
  }
  finish(')');
  start("for (auto " << o->index << " = " << o->from << "; ");
  middle(o->index << " <= " << o->to << "; ");
  finish("++" << o->index << ") {");
  in();
  *this << o->braces->strip();
  out();
  line("}");
  out();
  line("}");
}

void bi::CppBaseGenerator::visit(const While* o) {
  genTraceLine(o->loc);
  start("while (");
  auto cond = o->cond->strip();
  if (cond->type->isClass()) {
    middle("*(");
  }
  middle(cond);
  if (cond->type->isClass()) {
    middle(')');
  }
  finish(") {");
  in();
  *this << o->braces->strip();
  out();
  line("}");
}

void bi::CppBaseGenerator::visit(const DoWhile* o) {
  genTraceLine(o->loc);
  line("do {");
  in();
  *this << o->braces->strip();
  out();
  line("} while (" << o->cond->strip() << ");");
}

void bi::CppBaseGenerator::visit(const Assert* o) {
  genTraceLine(o->loc);
  line("libbirch_assert_(" << o->cond->strip() << ");");
}

void bi::CppBaseGenerator::visit(const Return* o) {
  genTraceLine(o->loc);
  line("return " << o->single << ';');
}

void bi::CppBaseGenerator::visit(const Yield* o) {
  assert(false);  // should be in CppFiberGenerator
}

void bi::CppBaseGenerator::visit(const Raw* o) {
  if ((header && *o->name == "hpp") || (!header && *o->name == "cpp")) {
    if (!header) {
      genSourceLine(o->loc);
    }
    *this << escape_unicode(o->raw);
    if (!std::isspace(o->raw.back())) {
      *this << ' ';
    }
  }
}

void bi::CppBaseGenerator::visit(const StatementList* o) {
  middle(o->head << o->tail);
}

void bi::CppBaseGenerator::visit(const EmptyType* o) {
  middle("void");
}

void bi::CppBaseGenerator::visit(const ArrayType* o) {
  middle("libbirch::DefaultArray<" << o->single << ',' << o->depth() << '>');
}

void bi::CppBaseGenerator::visit(const TupleType* o) {
  middle("libbirch::Tuple<" << o->single << '>');
}

void bi::CppBaseGenerator::visit(const FunctionType* o) {
  middle("std::function<" << o->returnType << '(' << o->params << ")>");
}

void bi::CppBaseGenerator::visit(const FiberType* o) {
  middle("libbirch::Fiber<" << o->single << '>');
}

void bi::CppBaseGenerator::visit(const OptionalType* o) {
  middle("libbirch::Optional<" << o->single << '>');
}

void bi::CppBaseGenerator::visit(const ClassType* o) {
  if (o->weak) {
    middle("libbirch::LazyWeakPtr<");
  } else {
    middle("libbirch::LazySharedPtr<");
  }
  middle("bi::type::" << o->name);
  if (!o->typeArgs->isEmpty()) {
    middle('<' << o->typeArgs << '>');
  }
  middle('>');
}

void bi::CppBaseGenerator::visit(const BasicType* o) {
  middle("bi::type::" << o->name);
}

void bi::CppBaseGenerator::visit(const GenericType* o) {
  if (o->target->type->isEmpty()) {
    middle(o->name);
  } else {
    middle(o->target->type);
  }
}

void bi::CppBaseGenerator::visit(const MemberType* o) {
  middle(o->right);
}

void bi::CppBaseGenerator::visit(const UnknownType* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const TypeList* o) {
  middle(o->head << ", " << o->tail);
}

void bi::CppBaseGenerator::genTraceFunction(const std::string& name,
    const Location* loc) {
  genSourceLine(loc);
  start("libbirch_function_(\"" << name << "\", \"");
  finish(loc->file->path << "\", " << loc->firstLine << ");");
}

void bi::CppBaseGenerator::genTraceLine(const Location* loc) {
  genSourceLine(loc);
  line("libbirch_line_(" << loc->firstLine << ");");
  genSourceLine(loc);
}

void bi::CppBaseGenerator::genSourceLine(const Location* loc) {
  line("#line " << loc->firstLine << " \"" << loc->file->path << "\"");
}

void bi::CppBaseGenerator::genArgs(const Expression* args, const Type* types) {
  auto iter1 = args->begin();
  auto end1 = args->end();
  auto iter2 = types->begin();
  auto end2 = types->end();
  while (iter1 != end1 && iter2 != end2) {
    if (iter1 != args->begin()) {
      middle(", ");
    }
    genArg(*iter1, *iter2);
    ++iter1;
    ++iter2;
  }
}

void bi::CppBaseGenerator::genLeftArg(const Call<BinaryOperator>* o) {
  genArg(o->args->getLeft(), o->target->params->getLeft()->type);
}

void bi::CppBaseGenerator::genRightArg(const Call<BinaryOperator>* o) {
  genArg(o->args->getRight(), o->target->params->getRight()->type);
}

void bi::CppBaseGenerator::genSingleArg(const Call<UnaryOperator>* o) {
  genArg(o->args, o->target->params->type);
}

void bi::CppBaseGenerator::genArg(const Expression* arg, const Type* type) {
  /* Birch and C++ resolve overloads differently, explicit casting avoids
   * situations where Birch considers a call unambiguous, whereas C++ does
   * not */
  auto isThis = dynamic_cast<const This*>(arg);
  auto isSuper = dynamic_cast<const Super*>(arg);
  auto isSequence = dynamic_cast<const Sequence*>(arg);
  if ((!arg->type->equals(*type) && arg->type->isConvertible(*type)) || isThis || isSuper || isSequence) {
    middle(type->canonical() << '(');
    if (!type->isValue()) {
      middle("context_, ");
    }
    middle(arg << ')');
  } else {
    /* either the same type, or using an assignment operator overload */
    middle(arg);
  }
}
