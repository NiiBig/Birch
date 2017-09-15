/**
 * @file
 */
#pragma once

#include "bi/common/Dictionary.hpp"
#include "bi/common/OverloadedDictionary.hpp"
#include "bi/common/Named.hpp"

#include <set>

namespace bi {
class Parameter;
class MemberParameter;
class GlobalVariable;
class LocalVariable;
class MemberVariable;
class Function;
class Fiber;
class MemberFunction;
class MemberFiber;
class Program;
class BinaryOperator;
class UnaryOperator;
class Basic;
class Class;
class Alias;
class Unknown;

template<class ObjectType> class Identifier;
template<class ObjectType> class OverloadedIdentifier;

class BasicType;
class ClassType;
class AliasType;
class IdentifierType;

/**
 * Categories of objects for identifier lookups.
 */
enum LookupResult {
  PARAMETER,
  MEMBER_PARAMETER,
  GLOBAL_VARIABLE,
  LOCAL_VARIABLE,
  MEMBER_VARIABLE,
  FUNCTION,
  FIBER,
  MEMBER_FUNCTION,
  MEMBER_FIBER,
  BASIC,
  CLASS,
  ALIAS,
  UNRESOLVED
};

/**
 * Scope.
 *
 * @ingroup compiler_common
 */
class Scope {
public:
  /**
   * Look up the category for an unknown identifier.
   */
  LookupResult lookup(const Identifier<Unknown>* ref) const;
  LookupResult lookup(const IdentifierType* ref) const;

  /**
   * Add declaration to scope.
   *
   * @param o Object.
   */
  void add(Parameter* o);
  void add(MemberParameter* o);
  void add(GlobalVariable* o);
  void add(LocalVariable* o);
  void add(MemberVariable* o);
  void add(Function* o);
  void add(Fiber* o);
  void add(Program* o);
  void add(MemberFunction* o);
  void add(MemberFiber* o);
  void add(BinaryOperator* o);
  void add(UnaryOperator* o);
  void add(Class* o);
  void add(Alias* o);
  void add(Basic* o);

  /**
   * Get the declaration to which an identifier corresponds.
   *
   * @param o Identnfier.
   *
   * @return Declaration.
   */
  void resolve(Identifier<Parameter>* o);
  void resolve(Identifier<MemberParameter>* o);
  void resolve(Identifier<GlobalVariable>* o);
  void resolve(Identifier<LocalVariable>* o);
  void resolve(Identifier<MemberVariable>* o);
  void resolve(OverloadedIdentifier<Function>* o);
  void resolve(OverloadedIdentifier<Fiber>* o);
  void resolve(Identifier<Program>* o);
  void resolve(OverloadedIdentifier<MemberFunction>* o);
  void resolve(OverloadedIdentifier<MemberFiber>* o);
  void resolve(OverloadedIdentifier<BinaryOperator>* o);
  void resolve(OverloadedIdentifier<UnaryOperator>* o);
  void resolve(BasicType* o);
  void resolve(ClassType* o);
  void resolve(AliasType* o);

  /**
   * Inherit another scope into this scope. This is used to inherit
   * declarations from a base class into a derived class.
   *
   * @param scope Scope to inherit.
   */
  void inherit(Scope* scope);

  /**
   * Base scope.
   */
  std::set<Scope*> bases;

  /*
   * Dictionaries.
   */
  Dictionary<Parameter> parameters;
  Dictionary<MemberParameter> memberParameters;
  Dictionary<GlobalVariable> globalVariables;
  Dictionary<LocalVariable> localVariables;
  Dictionary<MemberVariable> memberVariables;
  OverloadedDictionary<Function> functions;
  OverloadedDictionary<Fiber> fibers;
  Dictionary<Program> programs;
  OverloadedDictionary<MemberFunction> memberFunctions;
  OverloadedDictionary<MemberFiber> memberFibers;
  OverloadedDictionary<BinaryOperator> binaryOperators;
  OverloadedDictionary<UnaryOperator> unaryOperators;
  Dictionary<Basic> basics;
  Dictionary<Class> classes;
  Dictionary<Alias> aliases;

private:
  /**
   * Defer lookup to inherited scopes.
   */
  template<class ReferenceType>
  LookupResult lookupInherit(const ReferenceType* ref) const {
    LookupResult result = UNRESOLVED;
    for (auto iter = bases.begin();
        result == UNRESOLVED && iter != bases.end(); ++iter) {
      result = (*iter)->lookup(ref);
    }
    return result;
  }

  /**
   * Defer resolution to inherited scopes.
   */
  template<class ReferenceType>
  void resolveInherit(ReferenceType* ref) {
    for (auto iter = bases.begin(); !ref->target && iter != bases.end();
        ++iter) {
      (*iter)->resolve(ref);
    }
  }
};
}
