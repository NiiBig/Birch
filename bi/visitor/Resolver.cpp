/**
 * @file
 */
#include "bi/visitor/Resolver.hpp"

#include "bi/visitor/Instantiater.hpp"

bi::Resolver::Resolver(Scope* rootScope, const bool pointers) :
    pointers(pointers) {
  scopes.push_back(rootScope);
}

bi::Resolver::~Resolver() {
  //
}

bi::Expression* bi::Resolver::modify(ExpressionList* o) {
  Modifier::modify(o);
  o->type = new TypeList(o->head->type, o->tail->type, o->loc);
  return o;
}

bi::Expression* bi::Resolver::modify(Parentheses* o) {
  Modifier::modify(o);
  if (o->single->width() > 1) {
    o->type = new TupleType(o->single->type, o->loc);
  } else {
    o->type = o->single->type;
  }
  return o;
}

bi::Expression* bi::Resolver::modify(Sequence* o) {
  Modifier::modify(o);
  auto iter = o->single->type->begin();
  if (iter == o->single->type->end()) {
    o->type = new NilType(o->loc);
  } else {
    auto common = *iter;
    ++iter;
    while (common && iter != o->single->type->end()) {
      common = common->common(**iter);
      ++iter;
    }
    if (!common) {
      throw SequenceException(o);
    } else {
      o->type = new SequenceType(common, o->loc);
    }
  }
  return o;
}

bi::Expression* bi::Resolver::modify(Binary* o) {
  Modifier::modify(o);
  o->type = new BinaryType(o->left->type, o->right->type, o->loc);
  return o;
}

bi::Type* bi::Resolver::modify(UnknownType* o) {
  return lookup(o)->accept(this);
}

bi::Type* bi::Resolver::modify(ClassType* o) {
  assert(!o->target);
  Modifier::modify(o);
  resolve(o, GLOBAL_SCOPE);
  instantiate(o);
  return o;
}

bi::Type* bi::Resolver::modify(BasicType* o) {
  assert(!o->target);
  Modifier::modify(o);
  resolve(o, GLOBAL_SCOPE);
  return o;
}

bi::Type* bi::Resolver::modify(GenericType* o) {
  assert(!o->target);
  Modifier::modify(o);
  resolve(o, CLASS_SCOPE);
  return o;
}

bi::Type* bi::Resolver::modify(MemberType* o) {
  o->left = o->left->accept(this);
  if (o->left->isClass()) {
    memberScopes.push_back(o->left->getClass()->scope);
  } else {
    throw MemberException(o);
  }
  o->right = o->right->accept(this);

  return o;
}

bi::Expression* bi::Resolver::lookup(Identifier<Unknown>* ref) {
  LookupResult category = UNRESOLVED;
  if (!memberScopes.empty()) {
    /* use membership scope */
    category = memberScopes.back()->lookup(ref);
  } else {
    /* use current stack of scopes */
    for (auto iter = scopes.rbegin();
        category == UNRESOLVED && iter != scopes.rend(); ++iter) {
      category = (*iter)->lookup(ref);
    }
  }

  /* replace the reference of unknown object type with that of a known one */
  switch (category) {
  case PARAMETER:
    return new Identifier<Parameter>(ref->name, ref->loc);
  case GLOBAL_VARIABLE:
    return new Identifier<GlobalVariable>(ref->name, ref->loc);
  case LOCAL_VARIABLE:
    return new Identifier<LocalVariable>(ref->name, ref->loc);
  case MEMBER_VARIABLE:
    return new Identifier<MemberVariable>(ref->name, ref->loc);
  case FUNCTION:
    return new OverloadedIdentifier<Function>(ref->name, ref->loc);
  case FIBER:
    return new OverloadedIdentifier<Fiber>(ref->name, ref->loc);
  case MEMBER_FUNCTION:
    return new OverloadedIdentifier<MemberFunction>(ref->name, ref->loc);
  case MEMBER_FIBER:
    return new OverloadedIdentifier<MemberFiber>(ref->name, ref->loc);
  default:
    throw UnresolvedException(ref);
  }
}

bi::Type* bi::Resolver::lookup(UnknownType* ref) {
  LookupResult category = UNRESOLVED;
  if (!memberScopes.empty()) {
    /* use membership scope */
    category = memberScopes.back()->lookup(ref);
  } else {
    /* use current stack of scopes */
    for (auto iter = scopes.rbegin();
        category == UNRESOLVED && iter != scopes.rend(); ++iter) {
      category = (*iter)->lookup(ref);
    }
  }

  /* replace the reference of unknown object type with that of a known one */
  switch (category) {
  case BASIC:
    return new BasicType(ref->name, ref->loc);
  case CLASS:
    return new PointerType(ref->weak,
        new ClassType(ref->name, ref->typeArgs, ref->loc), ref->loc);
  case GENERIC:
    return new GenericType(ref->name, ref->loc);
  default:
    throw UnresolvedException(ref);
  }
}

void bi::Resolver::instantiate(ClassType* o) {
  if (o->target->isGeneric()) {
    if (o->typeArgs->width() != o->target->typeParams->width()) {
      throw GenericException(o, o->target);
    }
    Class* instantiation = o->target->getInstantiation(o->typeArgs);
    if (!instantiation) {
      Instantiater instantiater(o->typeArgs);
      instantiation = dynamic_cast<Class*>(o->target->accept(&instantiater));
      assert(instantiation);
      instantiation->isInstantiation = true;
      o->target->addInstantiation(instantiation);
      instantiation->accept(this);
    }
    o->target = instantiation;
  }
}

void bi::Resolver::checkBoolean(const Expression* o) {
  static BasicType type(new Name("Boolean"));
  scopes.front()->resolve(&type);
  if (!o->type->definitely(type)) {
    throw ConditionException(o);
  }
}

void bi::Resolver::checkInteger(const Expression* o) {
  static BasicType type(new Name("Integer"));
  scopes.front()->resolve(&type);
  if (!o->type->definitely(type)) {
    throw IndexException(o);
  }
}
