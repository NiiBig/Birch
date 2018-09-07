/**
 * @file
 */
#include "libbirch/Pool.hpp"

#include "libbirch/memory.hpp"

bi::Pool::Pool() :
    stack( { nullptr, 0u }) {
  //
}

bool bi::Pool::empty() const {
  return !stack.load().top;
}

void* bi::Pool::pop() {
  stack_t expected = stack.load();
  stack_t desired = { getNext(expected.top), expected.count + 1u };
  while (expected.top && !stack.compare_exchange_weak(expected, desired)) {
    desired = { getNext(expected.top), expected.count + 1u };
  }
  return expected.top;
}

void bi::Pool::push(void* block) {
  assert(bufferStart <= block && block < bufferStart + bufferSize);

  stack_t expected = stack.load();
  stack_t desired = { block, expected.count + 1u };
  setNext(block, expected.top);
  while (!stack.compare_exchange_weak(expected, desired)) {
    desired.count = expected.count + 1u;
    setNext(block, expected.top);
  }
}

void* bi::Pool::getNext(void* block) {
  assert(
      !block || (bufferStart <= block && block < bufferStart + bufferSize));

  return (block) ? *reinterpret_cast<void**>(block) : nullptr;
}

void bi::Pool::setNext(void* block, void* value) {
  assert(
      !block || (bufferStart <= block && block < bufferStart + bufferSize));
  assert(
      !value || (bufferStart <= value && value < bufferStart + bufferSize));

  if (block) {
    *reinterpret_cast<void**>(block) = value;
  }
}
