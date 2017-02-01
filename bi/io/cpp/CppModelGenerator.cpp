/**
 * @file
 */
#include "bi/io/cpp/CppModelGenerator.hpp"

#include "bi/io/cpp/CppBaseGenerator.hpp"
#include "bi/io/cpp/CppConstructorGenerator.hpp"
#include "bi/io/cpp/CppViewConstructorGenerator.hpp"
#include "bi/io/cpp/CppCopyConstructorGenerator.hpp"
#include "bi/io/cpp/CppAssignmentGenerator.hpp"
#include "bi/io/cpp/CppParameterGenerator.hpp"
#include "bi/io/cpp/CppOutputGenerator.hpp"
#include "bi/io/cpp/CppReturnGenerator.hpp"
#include "bi/io/cpp/misc.hpp"

bi::CppModelGenerator::CppModelGenerator(std::ostream& base, const int level,
    const bool header) :
    indentable_ostream(base, level, header),
    model(nullptr),
    inArray(false) {
  //
}

void bi::CppModelGenerator::visit(const Name* o) {
  *this << o->str();
}

void bi::CppModelGenerator::visit(const ModelParameter* o) {
  model = o;
  if (!o->isBuiltin()) {
    /* start boilerplate */
    if (header) {
      line("template<class Group = MemoryGroup>");
      start("class " << o->name);
      if (!o->base->isEmpty()) {
        middle(" : public " << o->base);
      }
      finish(" {");
      line("public:");
      in();
      line("typedef Group group_type;");
      line("typedef " << o->name << "<Group> value_type;");
      if (!o->base->isEmpty()) {
        line("typedef " << o->base << " base_type;");
      }
      line("");
    }

    /* constructor */
    CppConstructorGenerator auxConstructor(base, level, header);
    auxConstructor << o;

    /* copy constructor */
    if (header) {
      line(o->name << "(const " << o->name << "<Group>& o) = default;\n");
    }
    CppCopyConstructorGenerator auxCopyConstructor(base, level, header);
    auxCopyConstructor << o;

    /* move constructor */
    if (header) {
      line(o->name << '(' << o->name << "<Group>&& o) = default;\n");
    }

    /* view constructor */
    CppViewConstructorGenerator auxViewConstructor(base, level, header);
    auxViewConstructor << o;

    /* destructor */
    if (header) {
      line("virtual ~" << o->name << "() {");
      in();
      line("//");
      out();
      line("}\n");
    }

    /* copy assignment operator */
    CppAssignmentGenerator auxAssignment(base, level, header);
    auxAssignment << o;

    /* move assignment operator */
    if (header) {
      start(o->name->str() << "<Group>& ");
      finish("operator=(" << o->name->str() << "<Group>&& o) = default;\n");
    }

    /* view operator */
    if (header) {
      line("template<class Frame, class View>");
      line(
          o->name << "<Group> operator()(const Frame& frame, const View& view) {");
      in();
      line("return " << o->name << "<Group>(*this, frame, view);");
      out();
      line("}\n");
    }

    /* group member variable */
    if (header) {
      line("Group group;");
    }

    /* member variables and functions */
    *this << o->braces;

    /* end boilerplate */
    if (header) {
      out();
      line("};\n");
    }

    /* explicit template specialisations */
    if (!header) {
      line("template class bi::model::" << o->name << "<bi::MemoryGroup>;");
      line("template class bi::model::" << o->name << "<bi::NetCDFGroup>;");
      line("");
    }
  }
}

void bi::CppModelGenerator::visit(const ModelReference* o) {
  if (!header) {
    middle("bi::model::");
  }
  middle(o->name << "<Group>");
}

void bi::CppModelGenerator::visit(const BracketsType* o) {
  if (!header) {
    middle("bi::");
  }
  inArray = true;
  middle("DefaultArray<" << o->single << ',' << o->count() << '>');
  inArray = false;
}

void bi::CppModelGenerator::visit(const VarDeclaration* o) {
  if (header) {
    line(o->param << ';');
  }
}

void bi::CppModelGenerator::visit(const VarParameter* o) {
  middle(o->type << ' ' << o->name);
}

void bi::CppModelGenerator::visit(const FuncParameter* o) {
  if (!o->braces->isEmpty()) {
    /* class template parameters */
    if (!header) {
      line("template<class Group>");
    }

    /* return type */
    start("");
    CppBaseGenerator auxType(base, level, header);
    auxType << o->type << ' ';

    /* name */
    if (!header) {
      middle("bi::model::" << model->name << "<Group>::");
    }
    if ((o->isBinary() || o->isUnary()) && isTranslatable(o->name->str())
        && !o->parens->isRich()) {
      middle("operator" << translate(o->name->str()));
    } else {
      middle(o->mangled);
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
      CppOutputGenerator auxOutput(base, level, header);
      auxOutput << o;

      /* body */
      CppBaseGenerator auxBase(base, level, header);
      auxBase << o->braces;

      /* return statement */
      if (!o->result->isEmpty()) {
        CppReturnGenerator auxReturn(base, level, header);
        auxReturn << o;
      }

      out();
      finish("}\n");
    }
  }
}
