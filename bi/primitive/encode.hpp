/**
 * @file
 */
#pragma once

#include "bi/expression/FuncParameter.hpp"
#include "bi/expression/Expression.hpp"

#include <string>

namespace bi {
/**
 * Encode a string of ASCII 0-127 in base 32.
 */
void encode32(const std::string& in, std::string& out);

/**
 * Decode a string of base 32 to ASCII 0-127.
 */
void decode32(const std::string& in, std::string& out);

/**
 * Translate a single character from decoded to encoded.
 *
 * @param c Between 0 and 31.
 *
 * Maps to the characters @c [a-z0-5].
 */
unsigned char encode32(const unsigned char c);

/**
 * Translate a single character from encoded to decoded.
 */
unsigned char decode32(const unsigned char c);

/**
 * Generate a unique name for a function parameter.
 */
std::string uniqueName(const FuncParameter* o);

/**
 * Generate a unique name for a random parameter.
 */
std::string uniqueName(const Expression* o);

/**
 * Generate internal name. This is the original name, with any characters
 * outside the class [0-9a-zA-z_] translated to within that class, and an
 * underscore added to the end to avoid clashes with user variables.
 */
std::string internalName(const FuncParameter* o);

/**
 * Is character in the class [0-9a-zA-Z_]?
 */
bool isSimple(const char c);

/**
 * Escape special characters in a string.
 */
std::string escape(const std::string& str);
}
