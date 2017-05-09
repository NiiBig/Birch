/**
 * @file
 */
#pragma once

namespace bi {
/**
 * Signature form flags.
 */
enum SignatureForm {
  FUNCTION,
  BINARY_OPERATOR,
  UNARY_OPERATOR,
  ASSIGN_OPERATOR,
  LAMBDA_FUNCTION,
  MEMBER_FUNCTION
};

/**
 * Signature of a function.
 *
 * @ingroup compiler_expression
 */
class Formed {
public:
  /**
   * Constructor.
   *
   * @param form Signature form.
   */
  Formed(const SignatureForm form = FUNCTION);

  /**
   * Destructor.
   */
  virtual ~Formed() = 0;

  /**
   * Is this a binary operator?
   */
  bool isBinary() const;

  /**
   * Is this a unary operator?
   */
  bool isUnary() const;

  /**
   * Is this an assignment operator?
   */
  bool isAssign() const;

  /**
   * Is this a lambda function?
   */
  bool isLambda() const;

  /**
   * Is this a lambda function?
   */
  bool isMember() const;

  /**
   * Form.
   */
  SignatureForm form;
};
}
