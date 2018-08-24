/**
 * @file
 */
#include "libbirch/global.hpp"

#include "libbirch/World.hpp"

#include <cstdio>
#include <cstdlib>

std::atomic<char*> bi::buffer(heap());
bi::Pool bi::pool[64];
bi::World* bi::fiberWorld = new bi::World(0);
bool bi::fiberClone = false;
std::vector<bi::StackFrame> bi::stacktrace(32);
// ^ reserving a non-zero size seems necessary to avoid segfault here

void bi::abort() {
  abort("assertion failed");
}

void bi::unknown_option(const std::string& name) {
  printf("error: unknown option '%s'\n", name.c_str());
  std::string possible = name;
  std::replace(possible.begin(), possible.end(), '_', '-');
  if (name != possible) {
    printf("note: perhaps try '%s' instead?\n", possible.c_str());
  }
  std::exit(1);
}

void bi::abort(const std::string& msg) {
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
#endif
  std::exit(1);
}
