/**
 * @file
 */
#include "bi/common/OverloadedDictionary.hpp"

#include "bi/statement/Function.hpp"
#include "bi/statement/Coroutine.hpp"
#include "bi/statement/MemberFunction.hpp"
#include "bi/statement/MemberCoroutine.hpp"
#include "bi/statement/BinaryOperator.hpp"
#include "bi/statement/UnaryOperator.hpp"
#include "bi/statement/AssignmentOperator.hpp"

template<class ObjectType>
bi::OverloadedDictionary<ObjectType>::~OverloadedDictionary() {
  for (auto o : this->objects) {
    delete o.second;
  }
  this->objects.clear();
}

template<class ObjectType>
void bi::OverloadedDictionary<ObjectType>::add(ObjectType* o) {
  if (this->contains(o->name->str())) {
    this->get(o->name->str())->add(o);
  } else {
    Dictionary<Overloaded<ObjectType>>::add(new Overloaded<ObjectType>(o));
  }
}

/*
 * Explicit instantiations.
 */
template class bi::OverloadedDictionary<bi::Function>;
template class bi::OverloadedDictionary<bi::Coroutine>;
template class bi::OverloadedDictionary<bi::MemberFunction>;
template class bi::OverloadedDictionary<bi::MemberCoroutine>;
template class bi::OverloadedDictionary<bi::BinaryOperator>;
template class bi::OverloadedDictionary<bi::UnaryOperator>;
template class bi::OverloadedDictionary<bi::AssignmentOperator>;
