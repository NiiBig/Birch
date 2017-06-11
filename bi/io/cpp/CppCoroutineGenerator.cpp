/**
 * @file
 */
#include "bi/io/cpp/CppCoroutineGenerator.hpp"

#include "bi/io/cpp/CppParameterGenerator.hpp"
#include "bi/visitor/Gatherer.hpp"

bi::CppCoroutineGenerator::CppCoroutineGenerator(std::ostream& base,
    const int level, const bool header) :
    CppBaseGenerator(base, level, header),
    state(0) {
  //
}

void bi::CppCoroutineGenerator::visit(const FuncParameter* o) {
  /* pre-condition */
  assert(o->isCoroutine());

  /* supporting class */
  if (header) {
    line("namespace bi {");
    in();
    line("namespace func {");
    out();
    line("class Coroutine_" << o->name << "_ : public Coroutine<" << o->type << "> {");
    line("public:");
    in();
  }

  /* constructor, taking the arguments of the coroutine */
  if (!header) {
    start("bi::func::Coroutine_" << o->name << "_::");
  } else {
    start("");
  }
  middle("Coroutine_" << o->name << "_");

  CppParameterGenerator auxParameter(base, level, header);
  auxParameter << o;
  if (header) {
    finish(';');
  } else {
    if (o->parens->tupleSize() > 0) {
      finish(" :");
      in();
      Gatherer<VarParameter> gatherer;
      o->parens->accept(&gatherer);
      for (auto iter = gatherer.begin(); iter != gatherer.end(); ++iter) {
        const VarParameter* param = *iter;
        if (iter != gatherer.begin()) {
          finish(',');
        }
        start(param->name << '_' << param->number << '_' << '(' << param->name << ')');
      }
      out();
    }
    finish(" {");
    in();
    line("//");
    out();
    line("}\n");
  }

  /* call function */
  if (header) {
    start("virtual ");
  } else {
    start("");
  }
  ++inReturn;
  middle(o->type);
  --inReturn;
  middle(' ');
  if (!header) {
    middle("bi::func::Coroutine_" << o->name << "_::");
  }
  middle("operator()()");
  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();

    Gatherer<Return> gatherer;
    o->braces->accept(&gatherer);
    if (gatherer.size() > 0) {
      line("switch (state) {");
      in();
      for (int s = 0; s <= gatherer.size(); ++s) {
        line("case " << s << ": goto STATE" << s << ';');
      }
      out();
      line('}');
    }

    line("STATE0:");
    ++state;
    *this << o->braces;

    line("state = " << state << ';');
    out();
    finish("}\n");
  }

  if (header) {
    line("");
    out();
    line("private:");
    in();

    /* parameters and local variables as class member variables */
    Gatherer<VarParameter> gatherer;
    o->accept(&gatherer);
    for (auto iter = gatherer.begin(); iter != gatherer.end(); ++iter) {
      const VarParameter* param = *iter;
      line(param->type << ' ' << param->name << '_' << param->number << "_;");
    }

    out();
    line("};");
    in();
    line("}");
    out();
    line("}\n");
  }

  /* initialisation function */
  if (header) {
    line("namespace bi {");
      in();
      line("namespace func {");
      out();
  }

  /* return type */
  ++inReturn;
  start("bi::Pointer<bi::Coroutine<" << o->type << ">> ");
  --inReturn;

  /* name */
  if (!header) {
    middle("bi::func::");
  }
  middle(o->name);

  /* parameters */
  auxParameter << o;

  if (header) {
    finish(';');
  } else {
    finish(" {");
    in();
    start("return new (GC_MALLOC(sizeof(bi::func::Coroutine_" << o->name << "_))) bi::func::Coroutine_" << o->name << "_(");
    for (auto iter = o->parens->begin(); iter != o->parens->end(); ++iter) {
      if (iter != o->parens->begin()) {
        middle(", ");
      }
      const VarParameter* param = dynamic_cast<const VarParameter*>(*iter);
      assert(param);
      middle(param->name);
    }
    finish(");");
    out();
    finish("}\n");
  }
  if (header) {
      in();
      line("}");
      out();
    line("}\n");
  }
}

void bi::CppCoroutineGenerator::visit(const Return* o) {
  line("state = " << state << "; return " << o->single << ';');
  line("STATE" << state << ": ;");
  ++state;
}

void bi::CppCoroutineGenerator::visit(const VarReference* o) {
  middle(o->name << '_' << o->target->number << '_');
}

void bi::CppCoroutineGenerator::visit(const VarParameter* o) {
  if (o->type->isClass() || !o->parens->isEmpty() || o->type->count() > 0) {
    middle(o->name << '_' << o->number << '_');
  }
  if (o->type->isClass()) {
    TypeReference* type = dynamic_cast<TypeReference*>(o->type->strip());
    assert(type);
    middle(" = new (GC_MALLOC(sizeof(bi::type::" << type->name << "))) bi::type::" << type->name << '(');
  } else if (!o->parens->isEmpty() || o->type->count() > 0) {
    middle('(');
  }
  if (o->type->count() > 0) {
    BracketsType* type = dynamic_cast<BracketsType*>(o->type->strip());
    assert(type);
    middle("make_frame(" << type->brackets << ")");
  }
  if (!o->parens->isEmpty()) {
    if (o->type->count() > 0) {
      middle(", ");
    }
    middle(o->parens->strip());
  }
  if (o->type->isClass() || !o->parens->isEmpty() || o->type->count() > 0) {
    middle(')');
  }
  if (!o->value->isEmpty()) {
    if (o->type->isClass()) {
      ///@todo How to handle this case?
      assert(false);
    } else {
      middle(" = " << o->value);
    }
  }
}
