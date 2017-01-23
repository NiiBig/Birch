/**
 * @file
 */
#include "bi/statement/File.hpp"

#include "bi/visitor/all.hpp"

bi::File::File(const std::string& path, Statement* root) :
    path(path),
    root(root),
    state(UNRESOLVED) {
  //
}

bi::File::~File() {
  //
}

bi::File* bi::File::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

void bi::File::accept(Modifier* visitor) {
  visitor->modify(this);
}

void bi::File::accept(Visitor* visitor) const {
  visitor->visit(this);
}
