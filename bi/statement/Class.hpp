/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Parenthesised.hpp"
#include "bi/common/Based.hpp"
#include "bi/common/Braced.hpp"
#include "bi/common/Scoped.hpp"

#include <set>
#include <list>

namespace bi {
/**
 * Class.
 *
 * @ingroup compiler_type
 */
class Class: public Statement,
    public Named,
    public Parenthesised,
    public Based,
    public Braced,
    public Scoped {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param parens Constructor parameters.
   * @param base Base type.
   * @param baseParens Base type constructor arguments.
   * @param braces Braces.
   * @param loc Location.
   */
  Class(shared_ptr<Name> name, Expression* parens, Type* base,
      Expression* baseParens, Statement* braces, shared_ptr<Location> loc =
          nullptr);

  /**
   * Destructor.
   */
  virtual ~Class();

  void addSuper(const Type* o);
  bool hasSuper(const Type* o) const;
  void addConversion(const Type* o);
  bool hasConversion(const Type* o) const;
  void addAssignment(const Type* o);
  bool hasAssignment(const Type* o) const;

  virtual Statement* accept(Cloner* visitor) const;
  virtual Statement* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  using Statement::definitely;
  using Statement::possibly;

  virtual bool dispatchDefinitely(const Statement& o) const;
  virtual bool definitely(const Class& o) const;

  virtual bool dispatchPossibly(const Statement& o) const;
  virtual bool possibly(const Class& o) const;

  /**
   * Base type constructor arguments.
   */
  unique_ptr<Expression> baseParens;

private:
  /**
   * Super classes.
   */
  std::set<const Class*> supers;

  /**
   * Types to which this class can be converted.
   */
  std::list<const Type*> conversions;

  /**
   * Types that can be assigned to this class.
   */
  std::list<const Type*> assignments;
};
}
