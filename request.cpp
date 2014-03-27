#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "encoding.hpp"
#include "request.hpp"
#include "utils.hpp"

Request::Request(Transaction &transaction,
    std::vector<std::string>& header_lines, std::string body_in)
    : transaction_(transaction) {
  DEBUG_CTOR("Request");

  if (header_lines.size() == 0) {
    printf("Request: No header lines\n");
    return;
  }

  std::stringstream stream;
  stream << header_lines[0];
  stream >> method >> uri >> http_version;

  body = body_in;

  size_t query_position = uri.find_first_of('?');
  if (query_position != std::string::npos) {
    path = uri.substr(0, query_position);
    query = uri.substr(query_position + 1);
  } else {
    path = uri;
  }

  parseParameterString(query);
  if (body.length() > 0) {
    // TODO: Check www-form-urlencoded.
    parseParameterString(body);
  }

  std::cout << "method: " << method << std::endl;
  std::cout << "uri: " << uri << std::endl;
  std::cout << "http_version: " << http_version << std::endl;
  std::cout << "path: " << path << std::endl;
  std::cout << "query: " << query << std::endl;
  std::cout << "body: " << body << std::endl;
}

void Request::parseParameterString(std::string parameter_string) {
  if (parameter_string.size()) {
    StrVectorSharedPtr pairs = split_string(parameter_string, '&');

    for (auto entry : *pairs) {
      StrVectorSharedPtr pair = split_string(entry, '=');
      if (pair->size() == 2) {
        auto &key = (*pair)[0];
        auto &encoded_value = (*pair)[1];

        auto unencoded_value = uri_unencode_string(encoded_value);

        if (unencoded_value == encoded_value) {
          std::cout << key << "= " << unencoded_value << std::endl;
        } else {
          std::cout << key << "= " << unencoded_value << "  (" << encoded_value << ")" << std::endl;
        }

        params.insert({key, unencoded_value});

        if (!params_multi.count(key)) {
          std::vector<std::string> new_vector;
          params_multi[key] = new_vector; // TODO: CHECK MEMORY RELEASE.
        }
        params_multi[key].push_back(unencoded_value);
      }
    }
  }
}

Request::~Request() {
  DEBUG_DTOR("Request");
}

std::string Request::operator[](const std::string &param) {
  return params[param];
}
