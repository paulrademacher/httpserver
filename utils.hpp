#ifndef __HTTP_UTILS_HPP__
#define __HTTP_UTILS_HPP__

#include <sstream>
#include <string>
#include <vector>

namespace utils {

typedef std::shared_ptr<std::vector<std::string>> StrVectorSharedPtr;

StrVectorSharedPtr split_string(const std::string &string, char delim);

// Returns a substring.  If pos is beyond the end of the string, returns empty string.
std::string substr(const std::string &string, size_t pos, size_t len=std::string::npos);

}

#endif // __HTTP_UTILS_HPP__
