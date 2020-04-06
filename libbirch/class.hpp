/**
 * @file
 */
#pragma once

/**
 * @internal
 *
 * @def LIBBIRCH_VIRTUALS
 *
 * Declare common virtual functions for classes and fibers.
 */
#define LIBBIRCH_VIRTUALS(Name, Base...) \
  virtual void finish_(libbirch::Label* label) override { \
    this->accept_(libbirch::Finisher(label)); \
  } \
  \
  virtual void freeze_() override { \
    this->accept_(libbirch::Freezer()); \
  } \
  \
  virtual Name* copy_(libbirch::Label* label) const override { \
    auto src = static_cast<const void*>(this); \
    auto dst = libbirch::allocate(sizeof(*this)); \
    std::memcpy(dst, src, sizeof(*this)); \
    auto o = static_cast<Name*>(dst); \
    o->accept_(libbirch::Copier(label)); \
    return o; \
  } \
  \
  virtual Name* recycle_(libbirch::Label* label) override { \
    this->accept_(libbirch::Recycler(label)); \
    return this; \
  } \
  \
  virtual void discard_() override { \
    this->accept_(libbirch::Discarder()); \
  } \
  \
  virtual void restore_() override { \
    this->accept_(libbirch::Restorer()); \
  }

/**
 * @def LIBBIRCH_CLASS
 *
 * Boilerplate macro for classes to support lazy deep copy. The first
 * argument is the name of the class; this should exclude any generic type
 * arguments. The second argument is the base class; this should include any
 * generic type arguments. The macro should be placed in the public section
 * of the class.b
 *
 * LIBBIRCH_CLASS must be immediately followed by LIBBIRCH_MEMBERS, otherwise
 * the replacement code will have invalid syntax. For example:
 *
 *     class A : public B {
 *     public:
 *       LIBBIRCH_CLASS(A, B)
 *       LIBBIRCH_MEMBERS(x, y, z)
 *     private:
 *       int x, y, z;
 *     };
 *
 * The use of a variadic macro here supports base classes that contain
 * commas without special treatment, e.g.
 *
 *     LIBBIRCH_CLASS(A, B<T,U>)
 */
#define LIBBIRCH_CLASS(Name, Base...) \
  LIBBIRCH_VIRTUALS(Name, Base) \
  LIBBIRCH_ABSTRACT_CLASS(Name, Base)

/**
 * @def LIBBIRCH_ABSTRACT_CLASS
 *
 * Use in place of LIBBIRCH_CLASS when the containing class is abstract.
 */
#define LIBBIRCH_ABSTRACT_CLASS(Name, Base...) \
  virtual const char* getClassName() const { \
    return #Name; \
  } \
  \
  auto self() { \
    return libbirch::Lazy<libbirch::Init<Name>>(this, this->getLabel()); \
  } \
  \
  template<class Visitor> \
  void accept_(const Visitor& v) { \
    Base::accept_(v);

/**
 * @def LIBBIRCH_FIBER
 *
 * Use in place of LIBBIRCH_CLASS when the containing class is for a fiber.
 */
#define LIBBIRCH_FIBER(Name, Base...) \
  LIBBIRCH_VIRTUALS(Name, Base...) \
  \
  virtual const char* getClassName() const { \
    return #Name; \
  } \
  \
  template<class Visitor> \
  void accept_(const Visitor& v) { \
    Base::accept_(v);

/**
 * @def LIBBIRCH_FIBER
 *
 * Use in place of LIBBIRCH_CLASS when the containing class is for a member
 * fiber.
 */
#define LIBBIRCH_MEMBER_FIBER(Name, Base...) \
  LIBBIRCH_VIRTUALS(Name, Base...) \
  \
  virtual const char* getClassName() const { \
    return #Name; \
  } \
  \
  auto& self() { \
    return state_.template get<0>(); \
  } \
  \
  template<class Visitor> \
  void accept_(const Visitor& v) { \
    Base::accept_(v);

/**
 * @def LIBBIRCH_MEMBERS
 *
 * Boilerplate macro for classes to support lazy deep copy. The arguments
 * list all member variables of the class (and should not include member
 * variables of base classes---these should have their own LIBBIRCH_MEMBERS
 * macro use.
 *
 * LIBBIRCH_MEMBERS must be immediately preceded by LIBBIRCH_CLASS or
 * LIBBIRCH_ABSTRACT_CLASS, otherwise the replacement code will have invalid
 * syntax. For example:
 *
 *     class A : public B {
 *     public:
 *       LIBBIRCH_CLASS(A, B)
 *       LIBBIRCH_MEMBERS(x, y, z)
 *     private:
 *       int x, y, z;
 *     };
 */
#define LIBBIRCH_MEMBERS(members...) \
    v.visit(members); \
  }

#include "libbirch/Freezer.hpp"
#include "libbirch/Copier.hpp"
#include "libbirch/Recycler.hpp"
#include "libbirch/Discarder.hpp"
#include "libbirch/Restorer.hpp"
