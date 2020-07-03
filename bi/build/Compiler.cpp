/**
 * @file
 */
#include "Compiler.hpp"

#include "bi/birch.hpp"
#include "bi/visitor/all.hpp"
#include "bi/io/cpp/CppPackageGenerator.hpp"
#include "bi/io/bi_ostream.hpp"
#include "bi/io/cpp_ostream.hpp"
#include "bi/io/hpp_ostream.hpp"
#include "bi/lexer.hpp"

bi::Compiler* compiler = nullptr;
std::stringstream raw;

bi::Compiler::Compiler(Package* package, const fs::path& build_dir,
    const bool unity) :
    scope(new Scope(GLOBAL_SCOPE)),
    package(package),
    build_dir(build_dir),
    unity(unity) {
  //
}

void bi::Compiler::parse() {
  compiler = this;  // set global variable needed by parser for callbacks
  for (auto file : package->files) {
    raw.str("");
    auto fd = fopen(file->path.c_str(), "r");
    if (!fd) {
      throw FileNotFoundException(file->path);
    }
    this->file = file;  // member variable needed by GNU Bison parser
    yyrestart(fd);
    yyreset();
    try {
      yyparse();
    } catch (bi::Exception& e) {
      yyerror(e.msg.c_str());
    }
    yylex_destroy();
    fclose(fd);
    this->file = nullptr;
  }
  compiler = nullptr;
}

void bi::Compiler::resolve() {
  Spinner spinner;
  package->accept(&spinner);

  Transformer transformer;
  package->accept(&transformer);

  Checker checker;
  package->accept(&checker);

  Scoper scoper;
  package->accept(&scoper);

  Baser baser;
  package->accept(&baser);

  Resolver resolver;
  package->accept(&resolver);
}

void bi::Compiler::gen() {
  fs::path path;
  std::stringstream stream;

  bih_ostream bihOutput(stream);
  cpp_ostream cppOutput(stream);

  CppPackageGenerator hppPackageOutput(stream, 0, true);
  CppPackageGenerator cppPackageOutput(stream, 0, false);

  /* single *.bih header for whole package */
  stream.str("");
  bihOutput << package;
  path = build_dir / "bi" / tarname(package->name);
  path.replace_extension(".bih");
  write_all_if_different(path, stream.str());

  /* single *.hpp header for whole package */
  stream.str("");
  hppPackageOutput << package;
  cppPackageOutput << package;  // generic class and function definitions
  path = build_dir / "bi" / tarname(package->name);
  path.replace_extension(".hpp");
  write_all_if_different(path, stream.str());

  if (unity) {
    /* for a unity build, generated source goes into the one *.cpp file for
     * the whole package */
    stream.str("");
    for (auto file : package->sources) {
      cppOutput << file;
    }
    path = build_dir / "bi" / tarname(package->name);
    path.replace_extension(".cpp");
    write_all_if_different(path, stream.str());
  } else {
    /* for a non-unity build, generated source goes into a separate *.cpp
     * file for each *.bi file */
    for (auto file : package->sources) {
      stream.str("");
      cppOutput << file;
      path = build_dir / file->path;
      path.replace_extension(".cpp");
      write_all_if_different(path, stream.str());
    }
  }
}

void bi::Compiler::setRoot(Statement* root) {
  this->file->root = root;
}
