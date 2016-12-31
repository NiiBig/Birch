/**
 * @file
 */
#include "bi/io/cpp/CppBaseGenerator.hpp"

#include "bi/io/cpp/CppTemplateParameterGenerator.hpp"
#include "bi/io/cpp/CppParameterGenerator.hpp"
#include "bi/io/cpp/CppOutputGenerator.hpp"
#include "bi/io/cpp/CppReturnGenerator.hpp"
#include "bi/io/cpp/misc.hpp"

bi::CppBaseGenerator::CppBaseGenerator(std::ostream& base, const int level,
    const bool header) :
    indentable_ostream(base, level),
    header(header) {
  //
}

void bi::CppBaseGenerator::visit(const BoolLiteral* o) {
  *this << "bi::Boolean<bi::StackGroup>((unsigned char)" << o->str << ')';
}

void bi::CppBaseGenerator::visit(const IntLiteral* o) {
  *this << "bi::Integer<bi::StackGroup>((int64_t)" << o->str << ')';
}

void bi::CppBaseGenerator::visit(const RealLiteral* o) {
  *this << "bi::Real<bi::StackGroup>((double)" << o->str << ')';
}

void bi::CppBaseGenerator::visit(const StringLiteral* o) {
  *this << "bi::String<bi::StackGroup>(" << o->str << ')';
}

void bi::CppBaseGenerator::visit(const Name* o) {
  *this << o->str();
}

void bi::CppBaseGenerator::visit(const ExpressionList* o) {
  middle(o->head);
  if (o->tail) {
    middle(", " << o->tail);
  }
}

void bi::CppBaseGenerator::visit(const StatementList* o) {
  middle(o->head);
  if (o->tail) {
    middle(o->tail);
  }
}

void bi::CppBaseGenerator::visit(const TypeList* o) {
  middle(o->head);
  Type* tail = o->tail.get();
  TypeList* list = dynamic_cast<TypeList*>(tail);
  while (list) {
    middle(',' << list->head);
    tail = list->tail.get();
    list = dynamic_cast<TypeList*>(tail);
  }
  middle(',' << tail);
}

void bi::CppBaseGenerator::visit(const ParenthesesExpression* o) {
  if (o->expr->tupleSize() > 1) {
    middle("std::make_tuple");
  }
  middle('(' << o->expr << ')');
}

void bi::CppBaseGenerator::visit(const BracesExpression* o) {
  //finish('{');
  //in();
  *this << o->stmt;
  //out();
  //start('}');
}

void bi::CppBaseGenerator::visit(const BracketsExpression* o) {
  middle(o->expr << "(bi::make_view(" << o->brackets << "))");
}

void bi::CppBaseGenerator::visit(const Range* o) {
  middle("bi::make_range(" << o->left << ", " << o->right << ')');
}

void bi::CppBaseGenerator::visit(const This* o) {
  middle("*this");
}

void bi::CppBaseGenerator::visit(const Traversal* o) {
  const This* left = dynamic_cast<const This*>(o->left.get());
  if (left) {
    // tidier this way
    middle("this->" << o->right);
  } else {
    middle(o->left << '.' << o->right);
  }
}

void bi::CppBaseGenerator::visit(const VarReference* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const FuncReference* o) {
  if (o->isBinary() && isTranslatable(o->name->str())
      && !o->target->parens->isRich()) {
    assert(o->args.size() == 2);
    auto arg1 = *o->args.begin();
    auto arg2 = *(++o->args.begin());
    if (arg1->isPrimary()) {
      middle(arg1);
    } else {
      middle('(' << arg1 << ')');
    }
    middle(' ' << translate(o->name->str()) << ' ');
    if (arg2->isPrimary()) {
      middle(arg2);
    } else {
      middle('(' << arg2 << ')');
    }
  } else if (o->isUnary() && isTranslatable(o->name->str())
      && !o->target->parens->isRich()) {
    assert(o->args.size() == 1);
    auto iter = o->args.begin();
    middle(translate(o->name->str()) << ' ' << *iter);
  } else {
    middle(o->target->unique);
    if (o->isConstructor()) {
      middle("<StackGroup>");
    }
    middle('(');
    for (auto iter = o->args.begin(); iter != o->args.end(); ++iter) {
      if (iter != o->args.begin()) {
        middle(", ");
      }
      middle(*iter);
    }
    middle(')');
  }
}

void bi::CppBaseGenerator::visit(const RandomReference* o) {
  middle(o->name);
}

void bi::CppBaseGenerator::visit(const ModelReference* o) {
  if (o->count() > 0) {
    middle("bi::Array<" << o->name << "<bi::HeapGroup>,");
    middle("typename bi::DefaultFrame<" << o->count() << ">::type>");
  } else {
    middle("bi::" << o->name << "<bi::StackGroup>");
  }
}

void bi::CppBaseGenerator::visit(const VarParameter* o) {
  middle(o->type << ' ' << o->name);
  if (*o->parens || o->type->count() > 0) {
    middle('(');
  }
  if (*o->parens) {
    middle(o->parens->strip());
    if (o->type->count() > 0) {
      middle(", ");
    }
  }
  if (o->type->count() > 0) {
    ModelReference* type = dynamic_cast<ModelReference*>(o->type.get());
    assert(type);
    middle("make_frame(" << type->brackets << ")");
    if (*o->value) {
      middle(", " << o->value->strip());
    }
  }
  if (*o->parens || o->type->count() > 0) {
    middle(')');
  }
  if (*o->value) {
    middle(" = " << o->value);
  }
}

void bi::CppBaseGenerator::visit(const FuncParameter* o) {
  if (*o->braces) {
    /* template parameters */
    CppTemplateParameterGenerator auxTemplateParameter(base, level, header);
    auxTemplateParameter << o;

    /* type */
    start(o->type << ' ');

    /* name */
    if (!header) {
      middle("bi::");
    }
    if ((o->isBinary() || o->isUnary()) && isTranslatable(o->name->str())
        && !o->parens->isRich()) {
      middle("operator" << translate(o->name->str()));
    } else {
      middle(o->unique);
    }

    /* parameters */
    CppParameterGenerator auxParameter(base, level, header);
    auxParameter << o;

    if (header) {
      finish(';');
    } else {
      finish(" {");
      in();

      /* output parameters */
      CppOutputGenerator aux(base, level, header);
      aux << o;

      /* body */
      *this << o->braces;

      /* return statement */
      if (*o->result) {
        CppReturnGenerator aux(base, level, header);
        aux << o;
      }

      out();
      finish("}\n");
    }
  }
}

void bi::CppBaseGenerator::visit(const RandomParameter* o) {
  middle(o->type << ' ' << o->name << '(');
  middle(o->left << ", [&]() -> " << o->right->type << " { return " << o->right << "; }");
  middle(')');
}

void bi::CppBaseGenerator::visit(const ExpressionStatement* o) {
  line(o->expr << ';');
}

void bi::CppBaseGenerator::visit(const Conditional* o) {
  line("if " << o->cond << " {");
  in();
  *this << o->braces;
  out();
  if (*o->falseBraces) {
    line("} else {");
    in();
    *this << o->falseBraces;
    out();
  }
  line("}");
}

void bi::CppBaseGenerator::visit(const Loop* o) {
  line("while " << o->cond << " {");
  in();
  *this << o->braces;
  out();
  line("}");
}

void bi::CppBaseGenerator::visit(const Raw* o) {
  if ((header && o->name->str().compare("hpp") == 0)
      || (!header && o->name->str().compare("cpp") == 0)) {
    *this << o->raw;
    if (!std::isspace(o->raw.back())) {
      *this << ' ';
    }
  }
}

void bi::CppBaseGenerator::visit(const EmptyType* o) {
  middle("void");
}

void bi::CppBaseGenerator::visit(const ParenthesesType* o) {
  middle("std::tuple<" << o->type << ">");
}

void bi::CppBaseGenerator::visit(const RandomType* o) {
  middle("bi::random<");
  middle(o->left << ',' << o->right);
  middle(">");
}
