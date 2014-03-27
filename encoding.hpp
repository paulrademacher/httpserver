#ifndef __HTTP_ENCODING_HPP__
#define __HTTP_ENCODING_HPP__

#include <string>

std::string uri_encode_string(std::string &s);
std::string uri_unencode_string(std::string &s);

#endif // __HTTP_ENCODING_HPP__
