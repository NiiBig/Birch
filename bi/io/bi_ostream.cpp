/**
 * @file
 */
#include "bi/io/bi_ostream.hpp"

bi::bi_ostream::bi_ostream(std::ostream& base, const int level,
    const bool header) :
    indentable_ostream(base, level, header) {
  base << std::fixed;
  // ^ forces floating point representation of integers to have decimal
  //   places
}

void bi::bi_ostream::visit(const BoolLiteral* o) {
  *this << o->str;
}

void bi::bi_ostream::visit(const IntLiteral* o) {
  *this << o->str;
}

void bi::bi_ostream::visit(const RealLiteral* o) {
  *this << o->str;
}

void bi::bi_ostream::visit(const StringLiteral* o) {
  *this << o->str;
}

void bi::bi_ostream::visit(const Name* o) {
  *this << o->str();
}

void bi::bi_ostream::visit(const Path* o) {
  *this << o->str();
}

void bi::bi_ostream::visit(const ExpressionList* o) {
  *this << o->head << ", " << o->tail;
}

void bi::bi_ostream::visit(const StatementList* o) {
  *this << o->head << o->tail;
}

void bi::bi_ostream::visit(const TypeList* o) {
  *this << o->head << ", " << o->tail;
}

void bi::bi_ostream::visit(const ParenthesesExpression* o) {
  *this << '(' << o->expr << ')';
}

void bi::bi_ostream::visit(const BracesExpression* o) {
  *this << "{\n";
  in();
  *this << o->stmt;
  out();
  *this << indent << '}';
}

void bi::bi_ostream::visit(const RandomVariable* o) {
  *this << o->left << " ~ " << o->right;
}

void bi::bi_ostream::visit(const Range* o) {
  *this << o->left << ".." << o->right;
}

void bi::bi_ostream::visit(const Traversal* o) {
  *this << o->left << '.' << o->right;
}

void bi::bi_ostream::visit(const This* o) {
  *this << "this";
}

void bi::bi_ostream::visit(const BracketsExpression* o) {
  *this << o->expr << '[' << o->brackets << ']';
}

void bi::bi_ostream::visit(const VarReference* o) {
  *this << o->name;
}

void bi::bi_ostream::visit(const FuncReference* o) {
  if (o->isBinary()) {
    *this << o->getLeft() << ' ' << o->name << ' ' << o->getRight();
  } else {
    *this << o->name << o->parens;
  }
}

void bi::bi_ostream::visit(const ModelReference* o) {
  *this << o->name;
  if (*o->brackets) {
    *this << '[' << o->brackets << ']';
  } else if (o->count() > 0) {
    *this << '[';
    for (int i = 0; i < o->count(); ++i) {
      if (i != 0) {
        *this << ',';
      }
      *this << '0';
    }
    *this << ']';
  }
}

void bi::bi_ostream::visit(const ProgReference* o) {
  *this << o->name << o->parens;
}

void bi::bi_ostream::visit(const VarParameter* o) {
  *this << o->name << ':' << o->type;
  if (*o->parens) {
    *this << o->parens;
  }
  if (*o->value) {
    *this << " <- " << o->value;
  }
}

void bi::bi_ostream::visit(const FuncParameter* o) {
  if (o->isBinary()) {
    ExpressionList* list = dynamic_cast<ExpressionList*>(o->parens->strip());
    assert(list);
    *this << list->head << ' ' << o->name << ' ' << list->tail;
  } else {
    *this << o->name << o->parens;
  }
  if (*o->result) {
    *this << " -> " << o->result;
  }
}

void bi::bi_ostream::visit(const ModelParameter* o) {
  *this << o->name;
  if (*o->parens) {
    *this << o->parens;
  }
  if (*o->base) {
    *this << ' ' << o->op << ' ' << o->base;
  }
}

void bi::bi_ostream::visit(const ProgParameter* o) {
  *this << o->name << o->parens;
  if (!header && *o->braces) {
    *this << o->braces;
  } else {
    *this << ';';
  }
}

void bi::bi_ostream::visit(const ParenthesesType* o) {
  *this << '(' << o->type << ')';
}

void bi::bi_ostream::visit(const File* o) {
  *this << o->root;
}

void bi::bi_ostream::visit(const Import* o) {
  *this << indent << "import " << o->path << ";\n";
}

void bi::bi_ostream::visit(const ExpressionStatement* o) {
  *this << indent << o->expr << '\n';
}

void bi::bi_ostream::visit(const Conditional* o) {
  *this << indent << "if " << o->cond << ' ' << o->braces;
  if (*o->falseBraces) {
    *this << " else " << o->falseBraces;
  }
  *this << '\n';
}

void bi::bi_ostream::visit(const Loop* o) {
  *this << indent << "while " << o->cond << ' ' << o->braces << '\n';
}

void bi::bi_ostream::visit(const Raw* o) {
  *this << indent << o->name << " {{\n";
  *this << indent << o->raw << '\n';
  *this << indent << "}}\n";
}

void bi::bi_ostream::visit(const VarDeclaration* o) {
  *this << indent << o->param << ";\n";
}

void bi::bi_ostream::visit(const FuncDeclaration* o) {
  *this << indent << "function " << o->param;
  if (!header && *o->param->braces) {
    *this << o->param->braces;
  } else {
    *this << ';';
  }
  *this << "\n\n";
}

void bi::bi_ostream::visit(const ModelDeclaration* o) {
  *this << indent << "model " << o->param;
  if (*o->param->braces) {
    *this << o->param->braces;
  } else {
    *this << ';';
  }
  *this << "\n\n";
}

void bi::bi_ostream::visit(const ProgDeclaration* o) {
  *this << indent << "program " << o->param;
  if (!header && *o->param->braces) {
    *this << o->param->braces;
  } else {
    *this << ';';
  }
  *this << "\n\n";
}
