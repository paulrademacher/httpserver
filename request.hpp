#ifndef __HTTP_REQUEST_HPP__
#define __HTTP_REQUEST_HPP__

#include <vector>
#include <string>

#include "common.hpp"

class Request {
public:
  std::string method;
  std::string uri;
  std::string http_version;
  std::string path;  // URI up to query string.
  std::string query;  // Query string, without leading '?'.
  std::string body;
  std::map<std::string, std::string> params;
  std::map<std::string, std::vector<std::string>> params_multi;

  // Retrieves single query parameter.
  std::string operator[](const std::string &param);

  Request(Transaction &transaction,
      std::vector<std::string>& header_lines, std::string body);
  ~Request();

private:
  Transaction &transaction_;  // Owner.
  void parseParameterString(std::string parameter_string);

  // Prevent copy.
  Request(const Request &copy) = delete;
};

typedef std::shared_ptr<Request> RequestSharedPtr;

#endif // __HTTP_REQUEST_HPP__
