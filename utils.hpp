#ifndef __HTTP_UTILS_HPP__
#define __HTTP_UTILS_HPP__

#include <sstream>
#include <string>
#include <vector>

typedef std::shared_ptr<std::vector<std::string>> StrVectorSharedPtr;

StrVectorSharedPtr split_string(const std::string &string, char delim);

#endif // __HTTP_UTILS_HPP__
