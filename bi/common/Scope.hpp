/**
 * @file
 */
#pragma once

#include "bi/common/Dictionary.hpp"
#include "bi/common/OverloadedDictionary.hpp"
#include "bi/common/Named.hpp"

namespace bi {
class VarParameter;
class FuncParameter;
class RandomParameter;
class ModelParameter;
class ProgParameter;

class VarReference;
class FuncReference;
class RandomReference;
class ModelReference;
class ProgReference;

/**
 * Scope.
 *
 * @ingroup compiler_common
 */
class Scope {
public:
  /**
   * Constructor.
   *
   * @param outer Containing (outer) scope.
   */
  Scope(shared_ptr<Scope> outer = nullptr);

  /**
   * Destructor.
   */
  virtual ~Scope();

  /**
   * Does the scope contain the declaration?
   *
   * @param param Declaration.
   *
   * For functions, matching is done by signature. For all others, matching
   * is done by name only.
   */
  bool contains(VarParameter* param);
  bool contains(FuncParameter* param);
  bool contains(RandomParameter* param);
  bool contains(ModelParameter* param);
  bool contains(ProgParameter* param);

  /**
   * Add parameter.
   *
   * @param param Parameter.
   */
  void add(VarParameter* param);
  void add(FuncParameter* param);
  void add(RandomParameter* random);
  void add(ModelParameter* param);
  void add(ProgParameter* param);

  /**
   * Resolve a reference to a parameter.
   *
   * @param ref Reference to resolve.
   *
   * @return Target of the reference.
   */
  VarParameter* resolve(VarReference* ref);
  FuncParameter* resolve(FuncReference* ref);
  RandomParameter* resolve(RandomReference* ref);
  ModelParameter* resolve(ModelReference* ref);

  /**
   * Import from another scope into this scope.
   *
   * @param scope Scope to import.
   */
  void import(shared_ptr<Scope> scope);

  /**
   * Scope that encloses this one.
   */
  shared_ptr<Scope> outer;

  /**
   * Imported scopes.
   *
   * Raw pointers used here to ensure uniqueness.
   */
  std::set<Scope*> imports;

  /**
   * Overloaded declarations, by name.
   */
  Dictionary<VarParameter,VarReference> vars;
  Dictionary<ModelParameter,ModelReference> models;
  OverloadedDictionary<FuncParameter,FuncReference> funcs;
  Dictionary<RandomParameter,RandomReference> randoms;
  Dictionary<ProgParameter,ProgReference> progs;

private:
  /**
   * Defer resolution to outer or imported scopes.
   */
  template<class ParameterType, class ReferenceType>
  ParameterType* resolveDefer(ReferenceType* ref);
};
}

template<class ParameterType, class ReferenceType>
ParameterType* bi::Scope::resolveDefer(ReferenceType* ref) {
  ParameterType* target = nullptr;

  /* check imported scopes first; this means that within the scope of a model
   * that inherits from another, the parent model is checked before the outer
   * scope; for files this doesn't matter, as their scope scope is always a
   * root scope */
  auto iter = imports.begin();
  while (!target && iter != imports.end()) {
    try {
      target = (*iter)->resolve(ref);
    } catch (UnresolvedReferenceException e) {
      //
    }
    ++iter;
  }
  if (!target && outer) {
    try {
      target = outer->resolve(ref);
    } catch (UnresolvedReferenceException e) {
      //
    }
  }
  if (target) {
    return target;
  } else {
    throw UnresolvedReferenceException(ref);
  }
}
