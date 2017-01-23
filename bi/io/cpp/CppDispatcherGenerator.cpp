/**
 * @file
 */
#include "bi/io/cpp/CppDispatcherGenerator.hpp"

#include "bi/io/cpp/CppTemplateParameterGenerator.hpp"
#include "bi/io/cpp/CppParameterGenerator.hpp"
#include "bi/visitor/DispatchGatherer.hpp"
#include "bi/visitor/Gatherer.hpp"

bi::CppDispatcherGenerator::CppDispatcherGenerator(Scope* scope, std::ostream& base,
    const int level, const bool header) :
    CppBaseGenerator(base, level, header), scope(scope) {
  //
}

void bi::CppDispatcherGenerator::visit(const File* o) {
  DispatchGatherer gatherer(o->scope.get());
  o->accept(&gatherer);

  header = true;
  for (auto iter = gatherer.gathered.begin(); iter != gatherer.gathered.end();
      ++iter) {
    if (*(*iter)->name != "<-") {
      *this << *iter;
    }
  }

  header = false;
  for (auto iter = gatherer.gathered.begin(); iter != gatherer.gathered.end();
      ++iter) {
    if (*(*iter)->name != "<-") {
      *this << *iter;
    }
  }
}

void bi::CppDispatcherGenerator::visit(const ModelReference* o) {
  if (!o->assignable) {
    middle("const ");
  }
  if (o->count() > 0) {
    middle("DefaultArray<" << o->name << "<HeapGroup>," << o->count() << '>');
  } else {
    middle("bi::model::" << o->name << "<>");
  }
}

void bi::CppDispatcherGenerator::visit(const VarParameter* o) {
  middle(o->name);
}

void bi::CppDispatcherGenerator::visit(const FuncParameter* o) {
  int i;
  start("template<");
  for (i = 1; i <= o->inputs.size(); ++i) {
    if (i != 1) {
      middle(", ");
    }
    middle("class T" << i);
  }
  finish(">");
  if (header) {
    middle("static ");
  }
  start(o->type << " dispatch_" << o->number << "_(");

  i = 1;
  for (auto iter = o->inputs.begin(); iter != o->inputs.end(); ++iter, ++i) {
    if (iter != o->inputs.begin()) {
      middle(", ");
    }
    middle("T" << i << "&& " << (*iter)->name);
  }
  middle(')');
  if (header) {
    finish(";\n");
  } else {
    finish(" {");
    in();
    start("try { return bi::" << o->mangled << '(');
    for (auto iter = o->inputs.begin(); iter != o->inputs.end(); ++iter) {
      if (iter != o->inputs.begin()) {
        middle(", ");
      }
      middle("bi::cast<" << (*iter)->type << "&>(" << (*iter)->name << ')');
    }
    finish("); } catch (std::bad_cast) {}");

    std::list<FuncParameter*> parents;
    scope->parents(const_cast<FuncParameter*>(o), parents);
    for (auto iter = parents.begin(); iter != parents.end(); ++iter) {
      start("try { return ");
      possibly result = *const_cast<FuncParameter*>(o) <= **iter;  // needed to capture arguments
      assert(result != untrue);
      middle("dispatch_" << (*iter)->number << "_(");

      Gatherer<VarParameter> gatherer;
      (*iter)->parens->accept(&gatherer);
      for (auto iter2 = gatherer.gathered.begin(); iter2 != gatherer.gathered.end();
          ++iter2) {
        if (iter2 != gatherer.gathered.begin()) {
          middle(", ");
        }
        middle((*iter2)->arg);
      }
      finish("); } catch (std::bad_cast) {}");
    }
    line("throw std::bad_cast();");
    out();
    finish("}\n");
  }
}
