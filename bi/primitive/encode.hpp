/**
 * @file
 */
#pragma once

#include <string>

namespace bi {
/**
 * Encode a string of ASCII 0-127 in base 32.
 */
std::string encode32(const std::string& in);

/**
 * Decode a string of base 32 to ASCII 0-127.
 */
std::string decode32(const std::string& in);

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
 * Does this operator exist in C++?
 */
bool isTranslatable(const std::string& op);

/**
 * Is character in the class [0-9a-zA-Z_]?
 */
bool isSimple(const char c);

/**
 * Generate C++ name. This is the original name, with any characters
 * outside the class [0-9a-zA-z_] translated to within that class, and an
 * underscore added to the end to avoid clashes with user variables.
 */
std::string internalise(const std::string& name);

/**
 * Escape special characters in a string.
 */
std::string escape(const std::string& str);

/**
 * Process a documentation comment to detailed description.
 */
std::string detailed(const std::string& str);

/**
 * Process a documentation comment to brief description.
 */
std::string brief(const std::string& str);

/**
 * Process a documentation comment to a single line.
 */
std::string one_line(const std::string& str);

/**
 * Process a string into an anchor for Markdown.
 */
std::string anchor(const std::string& str, const int number = 0);
}
