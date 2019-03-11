/**
 * @file
 */
#include "libbirch/stacktrace.hpp"

std::vector<libbirch::StackFrame,libbirch::Allocator<libbirch::StackFrame>> libbirch::stacktrace;

libbirch::StackFunction::StackFunction(const char* func, const char* file,
    const int line) {
  stacktrace.push_back( { func, file, line });
}

libbirch::StackFunction::~StackFunction() {
  stacktrace.pop_back();
}

void libbirch::abort() {
  abort("assertion failed");
}

void libbirch::abort(const std::string& msg) {
  printf("error: %s\n", msg.c_str());
#ifndef NDEBUG
  printf("stack trace:\n");
  unsigned i = 0;
  for (auto iter = stacktrace.rbegin(); i < 20u && iter != stacktrace.rend();
      ++iter) {
    printf("    %-24s @ %s:%d\n", iter->func, iter->file, iter->line);
    ++i;
  }
  if (i < stacktrace.size()) {
    int rem = stacktrace.size() - i;
    printf("  + %d more\n", rem);
  }
  assert(false);
#else
  std::exit(1);
#endif
}
