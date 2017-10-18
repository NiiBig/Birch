/**
 * @file
 */
#pragma once

#include "bi/statement/Statement.hpp"
#include "bi/common/Named.hpp"
#include "bi/common/Numbered.hpp"
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
 * @ingroup compiler_statement
 */
class Class: public Statement,
    public Named,
    public Numbered,
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
  Class(Name* name, Expression* parens, Type* base,
      Expression* baseParens, Statement* braces, Location* loc =
          nullptr);

  /**
   * Destructor.
   */
  virtual ~Class();

  /**
   * Add a super type.
   */
  void addSuper(const Type* o);

  /**
   * Is the given type a super type of this?
   */
  bool hasSuper(const Type* o) const;

  /**
   * Add a conversion.
   */
  void addConversion(const Type* o);

  /**
   * Does this class have a conversion for the given type?
   */
  bool hasConversion(const Type* o) const;

  /**
   * Add an assignment.
   */
  void addAssignment(const Type* o);

  /**
   * Does this class have an assignment for the given type?
   */
  bool hasAssignment(const Type* o) const;

  virtual Statement* accept(Cloner* visitor) const;
  virtual Statement* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  /**
   * Base type constructor arguments.
   */
  Expression* baseParens;

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
