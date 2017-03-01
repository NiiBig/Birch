/**
 * @file
 */
#include "bi/io/cpp/CppBaseGenerator.hpp"

#include "bi/visitor/Gatherer.hpp"
#include "bi/io/cpp/misc.hpp"

#include <unordered_set>

bi::CppBaseGenerator::CppBaseGenerator(std::ostream& base, const int level,
    const bool header) :
    indentable_ostream(base, level),
    header(header),
    inArray(false) {
  //
}

void bi::CppBaseGenerator::visit(const BooleanLiteral* o) {
  *this << o->str;
}

void bi::CppBaseGenerator::visit(const IntegerLiteral* o) {
  *this << o->str;
}

void bi::CppBaseGenerator::visit(const RealLiteral* o) {
  *this << o->str;
}

void bi::CppBaseGenerator::visit(const StringLiteral* o) {
  *this << o->str;
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
  if (o->single->tupleSize() > 1) {
    middle("std::make_tuple");
  }
  middle('(' << o->single << ')');
}

void bi::CppBaseGenerator::visit(const BracesExpression* o) {
  //finish('{');
  //in();
  *this << o->single;
  //out();
  //start('}');
}

void bi::CppBaseGenerator::visit(const BracketsExpression* o) {
  middle(o->single << "(bi::make_view(" << o->brackets << "))");
}

void bi::CppBaseGenerator::visit(const Index* o) {
  middle("bi::make_index(" << o->single << ')');
}

void bi::CppBaseGenerator::visit(const Range* o) {
  middle("bi::make_range(" << o->left << ", " << o->right << ')');
}

void bi::CppBaseGenerator::visit(const This* o) {
  middle("*this");
}

void bi::CppBaseGenerator::visit(const RandomInit* o) {
  middle(o->left << ".init(" << o->right << ", ");
  in();
  in();
  finish("[](const " << o->right->type << "& m_) {");
  in();
  line("return sim_(m_);");
  out();
  start("}, ");
  genCapture(o->backward.get());
  finish("() {");
  in();
  line("return " << o->backward << ';');
  out();
  start("})");
  out();
  out();
}

void bi::CppBaseGenerator::visit(const Member* o) {
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
  if (*o->name == "<-") {
    /* assignment operator */
    middle(o->getLeft() << " = " << o->getRight());
  } else if (o->dispatcher) {
    middle("dispatch_" << o->dispatcher->mangled << "_" << o->dispatcher->number << "_");
    genArgs(const_cast<FuncReference*>(o), o->dispatcher);
  } else {
    if (o->isBinary() && isTranslatable(o->name->str())
        && !o->target->parens->isRich()) {
      middle(o->getLeft());
      middle(' ' << translate(o->name->str()) << ' ');
      middle(o->getRight());
    } else if (o->isUnary() && isTranslatable(o->name->str())
        && !o->target->parens->isRich()) {
      middle(translate(o->name->str()) << ' ' << o->getRight());
    } else {
      middle("bi::" << o->target->mangled);
      if (o->isConstructor()) {
        middle("<>");
      }
      genArgs(const_cast<FuncReference*>(o), o->target);
    }
  }
}

void bi::CppBaseGenerator::visit(const ModelReference* o) {
  if (o->isBuiltin() && !inArray) {
    if (*o->name == "Boolean") {
      middle("unsigned char");
    } else if (*o->name == "Real64" || *o->name == "Real") {
      middle("double");
    } else if (*o->name == "Real32") {
      middle("float");
    } else if (*o->name == "Integer64" || *o->name == "Integer") {
      middle("int64_t");
    } else if (*o->name == "Integer32") {
      middle("int32_t");
    } else if (*o->name == "String") {
      middle("std::string");
    } else {
      assert(false);
    }
  } else {
    middle("bi::model::" << o->name << "<>");
  }
}

void bi::CppBaseGenerator::visit(const VarParameter* o) {
  middle(o->type << ' ' << o->name);
  if (!o->parens->isEmpty() || o->type->count() > 0) {
    middle('(');
  }
  if (!o->parens->isEmpty()) {
    middle(o->parens->strip());
    if (o->type->count() > 0) {
      middle(", ");
    }
  }
  if (o->type->count() > 0) {
    BracketsType* type = dynamic_cast<BracketsType*>(o->type.get());
    assert(type);
    middle("make_frame(" << type->brackets << ")");
    if (!o->value->isEmpty()) {
      middle(", " << o->value->strip());
    }
  }
  if (!o->parens->isEmpty() || o->type->count() > 0) {
    middle(')');
  }
  if (!o->value->isEmpty()) {
    middle(" = " << o->value);
  }
}

void bi::CppBaseGenerator::visit(const ExpressionStatement* o) {
  line(o->single << ';');
}

void bi::CppBaseGenerator::visit(const Conditional* o) {
  line("if " << o->cond << " {");
  in();
  *this << o->braces;
  out();
  if (!o->falseBraces->isEmpty()) {
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
  if ((header && *o->name == "hpp") || (!header && *o->name == "cpp")) {
    *this << o->raw;
    if (!std::isspace(o->raw.back())) {
      *this << ' ';
    }
  }
}

void bi::CppBaseGenerator::visit(const EmptyType* o) {
  middle("void");
}

void bi::CppBaseGenerator::visit(const BracketsType* o) {
  inArray = true;
  if (!o->assignable) {
    middle("const ");
  }
  middle("bi::Array<" << o->single << ',');
  inArray = false;
  middle("typename bi::DefaultFrame<" << o->count() << ">::type>");
}

void bi::CppBaseGenerator::visit(const ParenthesesType* o) {
  if (dynamic_cast<TypeList*>(o->single->strip())) {
    middle("std::tuple<" << o->single->strip() << ">");
  } else {
    middle(o->single);
  }
}

void bi::CppBaseGenerator::visit(const RandomType* o) {
  inArray = true;
  middle("bi::Random<" << o->left << ',' << o->right << '>');
  inArray = false;
}

void bi::CppBaseGenerator::visit(const VariantType* o) {
  middle("boost::variant<");
  for (auto iter = o->types.begin(); iter != o->types.end(); ++iter) {
    if (iter != o->types.begin()) {
      middle(',');
    }
    middle(*iter);
  }
  middle(">");
}

void bi::CppBaseGenerator::genCapture(const Expression* o) {
  /* for lambda, capture assignable variables by reference, others by value */
  Gatherer<VarReference> gatherer;
  o->accept(&gatherer);
  std::unordered_set<std::string> done;

  middle("[=");
  for (auto iter = gatherer.begin(); iter != gatherer.end(); ++iter) {
    const VarReference* ref = *iter;
    if (done.find(ref->name->str()) == done.end()) {
      if (ref->type->isRandom()) {
        middle(", &" << ref->name);
      }
      done.insert(ref->name->str());
    }
  }
  middle(']');
}

void bi::CppBaseGenerator::genArgs(Expression* ref, FuncParameter* param) {
  bool result = ref->definitely(*param);  // needed to capture arguments
  assert(result);
  Gatherer<VarParameter> gatherer;
  param->parens->accept(&gatherer);

  middle('(');
  for (auto iter = gatherer.begin(); iter != gatherer.end(); ++iter) {
    if (iter != gatherer.begin()) {
      middle(", ");
    }
    middle((*iter)->arg);
  }
  middle(')');
}

void bi::CppBaseGenerator::genArgs(Expression* ref, Dispatcher* param) {
  bool result = ref->possibly(*param);  // needed to capture arguments
  assert(result);
  Gatherer<VarParameter> gatherer;
  param->parens->accept(&gatherer);

  middle('(');
  for (auto iter = gatherer.begin(); iter != gatherer.end(); ++iter) {
    if (iter != gatherer.begin()) {
      middle(", ");
    }
    middle((*iter)->arg);
  }
  middle(')');
}
