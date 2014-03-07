#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "request.hpp"
#include "utils.hpp"

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

  if (query.size()) {
    // Parse query parameters.
    StrVectorSharedPtr params = split_string(query, '&');

    for (std::string param : *params) {
      StrVectorSharedPtr pair = split_string(param, '=');
      if (pair->size() == 2) {
        std::string &key = (*pair)[0];
        std::string &value = (*pair)[1];

        query_params.insert({key, value});

        if (!query_params_multi.count(key)) {
          std::vector<std::string> new_vector;
          query_params_multi[key] = new_vector; // TODO: CHECK MEMORY RELEASE.
        }
        query_params_multi[key].push_back(value);
      }
    }
  }

  std::cout << "method: " << method << std::endl;
  std::cout << "uri: " << uri << std::endl;
  std::cout << "http_version: " << http_version << std::endl;
  std::cout << "path: " << path << std::endl;
  std::cout << "query: " << query << std::endl;
}
