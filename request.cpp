#include <iostream>
#include <sstream>

#include "request.hpp"

Request::Request(std::vector<std::string>& header_lines, std::string content) {
  std::stringstream stream;
  stream << header_lines[0];
  stream >> method >> uri >> http_version;

  size_t query_position = uri.find_first_of('?');
  if (query_position != std::string::npos) {
    path = uri.substr(0, query_position);
    query = uri.substr(query_position + 1);
  } else {
    path = uri;
  }

  std::cout << "method: " << method << std::endl;
  std::cout << "uri: " << uri << std::endl;
  std::cout << "http_version: " << http_version << std::endl;
  std::cout << "path: " << path << std::endl;
  std::cout << "query: " << query << std::endl;
}
