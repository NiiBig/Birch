/**
 * @file
 */
#include "bi/visitor/IsValue.hpp"

bi::IsValue::IsValue() : result(true) {

}

void bi::IsValue::visit(const ClassType* o) {
  result = false;
}

void bi::IsValue::visit(const GenericType* o) {
  o->target->type->accept(this);
}

void bi::IsValue::visit(const Call<Function>* o) {
  if (done.find(o->target) != done.end()) {
    done.insert(o->target);
    o->target->accept(this);
  }
}

void bi::IsValue::visit(const Call<Fiber>* o) {
  if (done.find(o->target) != done.end()) {
    done.insert(o->target);
    o->target->accept(this);
  }
}
