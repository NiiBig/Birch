/**
 * @file
 */
#include "bi/common/Scope.hpp"

#include "bi/expression/VarParameter.hpp"
#include "bi/expression/FuncParameter.hpp"
#include "bi/type/ModelParameter.hpp"
#include "bi/exception/all.hpp"
#include "bi/visitor/Cloner.hpp"

bi::Scope::Scope(shared_ptr<Scope> outer) :
    outer(outer) {
  //
}

bi::Scope::~Scope() {
  //
}

bool bi::Scope::contains(VarParameter* param) {
  return vars.contains(param);
}

bool bi::Scope::contains(FuncParameter* param) {
  return funcs.contains(param);
}

bool bi::Scope::contains(ModelParameter* param) {
  return models.contains(param);
}

bool bi::Scope::contains(ProgParameter* param) {
  return progs.contains(param);
}

void bi::Scope::add(VarParameter* param) {
  vars.add(param);
}

void bi::Scope::add(FuncParameter* param) {
  funcs.add(param);
}

void bi::Scope::add(ModelParameter* param) {
  models.add(param);
}

void bi::Scope::add(ProgParameter* prog) {
  progs.add(prog);
}

bi::VarParameter* bi::Scope::resolve(const VarReference* ref) {
  try {
    return vars.resolve(ref);
  } catch (UnresolvedReferenceException e) {
    return resolveDefer<VarParameter,VarReference>(ref);
  }
}

bi::FuncParameter* bi::Scope::resolve(const FuncReference* ref) {
  try {
    return funcs.resolve(ref);
  } catch (UnresolvedReferenceException e) {
    return resolveDefer<FuncParameter,FuncReference>(ref);
  }
}

bi::ModelParameter* bi::Scope::resolve(const ModelReference* ref) {
  try {
    return models.resolve(ref);
  } catch (UnresolvedReferenceException e) {
    return resolveDefer<ModelParameter,ModelReference>(ref);
  }
}

void bi::Scope::import(shared_ptr<Scope> scope) {
  imports.insert(scope.get());
}
