/**
 * @file
 */
#include "bi/data/group.hpp"

bi::StackGroup bi::childGroup(const StackGroup& parent, const char* name) {
  return StackGroup();
}

bi::RefGroup bi::childGroup(const RefGroup& parent, const char* name) {
  return RefGroup();
}

bi::HeapGroup bi::childGroup(const HeapGroup& parent, const char* name) {
  return HeapGroup();
}

bi::NetCDFGroup bi::childGroup(const NetCDFGroup& parent, const char* name) {
  if (parent.mode == NEW || parent.mode == REPLACE) {
    return parent.createGroup(name);
  } else {
    return parent.mapGroup(name);
  }
}

bi::HeapGroup bi::arrayGroup(const StackGroup& parent, const char* name) {
  return HeapGroup();
}

bi::RefGroup bi::arrayGroup(const RefGroup& parent, const char* name) {
  return RefGroup();
}

bi::HeapGroup bi::arrayGroup(const HeapGroup& parent, const char* name) {
  return HeapGroup();
}

bi::NetCDFGroup bi::arrayGroup(const NetCDFGroup& parent, const char* name) {
  return parent;
}
