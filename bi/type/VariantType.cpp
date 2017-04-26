/**
 * @file
 */
#include "bi/type/VariantType.hpp"

#include "bi/type/TypeParameter.hpp"
#include "bi/visitor/all.hpp"

#include <algorithm>

bi::VariantType::VariantType(Type* definite,
    const std::list<Type*>& possibles, shared_ptr<Location> loc,
    const bool assignable) :
    Type(loc, assignable),
    definite(definite),
    possibles(possibles) {
  //
}

bi::VariantType::~VariantType() {
  //
}

void bi::VariantType::add(Type* o) {
  Type* o1 = o->strip();
  if (o1->isVariant()) {
    VariantType* variant = dynamic_cast<VariantType*>(o1);
    assert(variant);
    add(variant->definite.release());
    for (auto iter = variant->possibles.begin(); iter != variant->possibles.end(); ++iter) {
      add(*iter);
    }
    delete o;
  } else {
    auto f = [&](Type* type) {
      return o1->definitely(*type);
    };
    if (!o1->definitely(*definite)
        && !std::any_of(possibles.begin(), possibles.end(), f)) {
      possibles.push_back(o1);
    } else {
      delete o;
    }
  }
}

int bi::VariantType::size() const {
  return possibles.size() + 1;
}

bi::Type* bi::VariantType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::VariantType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::VariantType::accept(Visitor* visitor) const {
  visitor->visit(this);
}

bool bi::VariantType::isVariant() const {
  return true;
}

bool bi::VariantType::possiblyAny(const Type& o) const {
  auto f = [&](Type* o1) {
    return o1->possibly(o) || o1->definitely(o);
  };
  return f(definite.get()) || std::any_of(possibles.begin(), possibles.end(), f);
}

bool bi::VariantType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::VariantType::definitely(const BracketsType& o) const {
  return definite->definitely(o);
}

bool bi::VariantType::definitely(const EmptyType& o) const {
  return definite->definitely(o);
}

bool bi::VariantType::definitely(const LambdaType& o) const {
  return definite->definitely(o);
}

bool bi::VariantType::definitely(const List<Type>& o) const {
  return definite->definitely(o);
}

bool bi::VariantType::definitely(const TypeParameter& o) const {
  return definite->definitely(o);
}

bool bi::VariantType::definitely(const TypeReference& o) const {
  return definite->definitely(o);
}

bool bi::VariantType::definitely(const ParenthesesType& o) const {
  return definite->definitely(o);
}

bool bi::VariantType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::VariantType::possibly(const BracketsType& o) const {
  return possiblyAny(o);
}

bool bi::VariantType::possibly(const EmptyType& o) const {
  return possiblyAny(o);
}

bool bi::VariantType::possibly(const LambdaType& o) const {
  return possiblyAny(o);
}

bool bi::VariantType::possibly(const List<Type>& o) const {
  return possiblyAny(o);
}

bool bi::VariantType::possibly(const TypeParameter& o) const {
  return possiblyAny(o);
}

bool bi::VariantType::possibly(const TypeReference& o) const {
  return possiblyAny(o);
}

bool bi::VariantType::possibly(const ParenthesesType& o) const {
  return possiblyAny(o);
}
