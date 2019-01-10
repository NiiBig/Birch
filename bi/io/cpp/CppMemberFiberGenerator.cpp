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
  /* generate a unique name (within this scope) for the state of the fiber */
  std::stringstream base;
  bih_ostream buf(base);
  buf << o->params;
  std::string baseName = internalise(o->name->str()) + encode32(base.str());
  std::string stateName = baseName + "_FiberState";

  /* gather important objects */
  o->params->accept(&params);
  o->braces->accept(&locals);
  o->braces->accept(&yields);

  /* supporting class for state */
  if (header) {
    start("class " << stateName << " : ");
    finish("public FiberState<" << o->returnType->unwrap() << "> {");
    line("public:");
    in();
    line("using class_type = " << stateName << ';');
    line("using super_type = FiberState<" << o->returnType->unwrap() << ">;\n");
    start("SharedCOW<" << type->name);
    genTemplateArgs(type);
    finish("> self;");
    for (auto param : params) {
      line(param->type << ' ' << param->name << ';');
    }
    for (auto local : locals) {
      start(local->type << ' ');
      finish(getName(local->name->str(), local->number) << ';');
    }
  }

  /* self-reference function */
  if (header) {
    out();
    line("protected:");
    in();
  }

  /* constructor */
  start("");
  if (!header) {
    middle("bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::" << stateName << "::");
  }
  middle(stateName << "(const SharedCOW<");
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
    start("super_type(0, " << (yields.size() + 1) << ')');
    finish(',');
    start("self(self)");
    for (auto param : params) {
      finish(',');
      start(param->name << '(' << param->name << ')');
    }
    finish(" {");
    out();
    line("//");
    out();
    line("}\n");
  }

  /* copy constructor, destructor, assignment operator */
  if (header) {
    line(stateName << "(const " << stateName << "&) = default;");
    line("virtual ~" << stateName << "() = default;");
    line(stateName << "& operator=(const " << stateName << "&) = default;");
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
    start("void bi::type::" << type->name);
    genTemplateArgs(type);
    middle("::" << stateName << "::");
  }
  middle("freeze()");
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    line("if (!this->isFrozen()) {");
    in();
    line("super_type::freeze();");
    line("bi::freeze(self);");
    line("bi::freeze(value);");
    for (auto param : params) {
      line("bi::freeze(" << param->name << ");");
    }
    for (auto local : locals) {
      line("bi::freeze(" << getName(local->name->str(), local->number) << ");");
    }
    out();
    line("}");
    out();
    line("}\n");
  }

  /* query function */
  if (header) {
    start("virtual ");
  } else {
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
    line("SwapContext swap(context.get());");
    line("SharedCOW<class_type> local(this);");
    genSwitch();
    *this << o->braces->strip();
    genEnd();
    out();
    finish("}\n");
  }
  if (header) {
    line("};\n");
  }

  /* initialisation function */
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
  middle(o->name << '(' << o->params << ')');
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    line("SwapContext swap(context.get());");
    line("SharedCOW<this_type> self(this);");
    start("return make_fiber<" << stateName << ">(");
    middle("self");
    for (auto param: params) {
      middle(", ");
      middle(param->name);
    }
    finish(");");
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
