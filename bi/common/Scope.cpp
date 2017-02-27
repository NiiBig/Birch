/**
 * @file
 */
#include "bi/common/Scope.hpp"

#include "bi/expression/VarParameter.hpp"
#include "bi/expression/FuncParameter.hpp"
#include "bi/type/ModelParameter.hpp"
#include "bi/exception/all.hpp"
#include "bi/visitor/Cloner.hpp"

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
  if (vars.contains(param)) {
    throw PreviousDeclarationException(param, vars.get(param));
  } else {
    vars.add(param);
  }
}

void bi::Scope::add(FuncParameter* param) {
  if (funcs.contains(param)) {
    throw PreviousDeclarationException(param, funcs.get(param));
  } else {
    funcs.add(param);
  }
}

void bi::Scope::add(Dispatcher* param) {
  if (dispatchers.contains(param)) {
    throw PreviousDeclarationException(param, dispatchers.get(param));
  } else {
    dispatchers.add(param);
  }
}

void bi::Scope::add(ModelParameter* param) {
  if (models.contains(param)) {
    throw PreviousDeclarationException(param, models.get(param));
  } else {
    models.add(param);
  }
}

void bi::Scope::add(ProgParameter* param) {
  if (progs.contains(param)) {
    throw PreviousDeclarationException(param, progs.get(param));
  } else {
    progs.add(param);
  }
}

void bi::Scope::resolve(VarReference* ref) {
  ref->target = vars.resolve(ref);
  if (!ref->target) {
    resolveDefer<VarParameter,VarReference>(ref);
  }
}

void bi::Scope::resolve(FuncReference* ref) {
  ref->target = funcs.resolve(ref);
  if (!ref->target) {
    resolveDefer<FuncParameter,FuncReference>(ref);
  } else {
    ref->dispatcher = resolveDispatcher(ref);
  }
}

void bi::Scope::resolve(ModelReference* ref) {
  ref->target = models.resolve(ref);
  if (!ref->target) {
    resolveDefer<ModelParameter,ModelReference>(ref);
  }
}

bi::Dispatcher* bi::Scope::resolveDispatcher(FuncParameter* o) {
  Dispatcher* dispatcher = dispatchers.resolve(o);

  /* for a function to match a dispatcher, it must be possibly equivalent
   * to all other functions contained in that dispatcher, and all of those
   * functions must be possibly equivalent to it, the resolution above only
   * does the former, the second condition below ensures the latter. */
  if (dispatcher && dispatcher->possibly(*o)) {
    return dispatcher;
  } else {
    return nullptr;
  }
}

bi::Dispatcher* bi::Scope::resolveDispatcher(FuncReference* o) {
  return dispatchers.resolve(o);
}

void bi::Scope::inherit(Scope* scope) {
  bases.insert(scope);
}

void bi::Scope::import(Scope* scope) {
  vars.merge(scope->vars);
  funcs.merge(scope->funcs);
  dispatchers.merge(scope->dispatchers);
  models.merge(scope->models);
  progs.merge(scope->progs);
}

bi::Dispatcher* bi::Scope::parent(Dispatcher* dispatcher) {
  std::list<Dispatcher*> parents;
  dispatchers.parents(dispatcher, parents);
  assert(parents.size() <= 1);
  if (parents.size() == 1) {
    return parents.front();
  } else {
    return nullptr;
  }
}
