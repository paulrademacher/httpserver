#include <cctype>
#include <iostream>

#include "encoding.hpp"

bool is_unreserved_char(char c) {
  // http://en.wikipedia.org/wiki/Percent-encoding
  return
    (c >= 'A' && c <= 'Z') ||
    (c >= 'a' && c <= 'z') ||
    (c >= '0' && c <= '9') ||
    c == '-' ||
    c == '_' ||
    c == '.' ||
    c == '~';
}

/**
 * Converts a hex character to a numeric value.
 * Assumes input is a valid hex character.
 */
char char_hex_to_numeric(char hex) {
  if (hex >= 'a' && hex <= 'f') {
    return hex - 'a' + 10;
  }

  if (hex >= 'A' && hex <= 'F') {
    return hex - 'A' + 10;
  }

  return hex - '0';
}

/**
 * Converts a pair of hex characters into a numeric value.
 * Returns 0 if the input chars are not both hex digits.
 */
char two_char_hex_to_numeric(char hex_high, char hex_low) {
  if (!isxdigit(hex_high) || !isxdigit(hex_low)) {
    return 0;
  }
  return char_hex_to_numeric(hex_high) * 16 + char_hex_to_numeric(hex_low);
}

std::string uri_encode_string(std::string &unencoded) {
  std::string encoded;

  for (char c : unencoded) {
    if (is_unreserved_char(c)) {
      encoded += c;
    } else {
      // TODO
    }
  }

  return encoded;
}

std::string uri_unencode_string(std::string &encoded) {
  std::string unencoded;

  for (int i = 0; i < encoded.length(); i++) {
    auto c = encoded[i];
    if (c == '%') {
      if (i + 2 < encoded.length()) {
        char unencoded_char = two_char_hex_to_numeric(encoded[i+1], encoded[i+2]);
        if (unencoded_char != 0) {  // 0 signals bad (non-hex) input.
          unencoded += unencoded_char;
        }
      }
      i += 2;
    } else if (c == '+') {
      unencoded += ' ';
    } else {
      unencoded += c;
    }
  }

  return unencoded;
}
