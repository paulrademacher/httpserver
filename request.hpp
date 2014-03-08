#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <vector>
#include <string>

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

  Request(std::vector<std::string>& header_lines, std::string body);

private:
  void parseParameterString(std::string parameter_string);
};

typedef std::shared_ptr<Request> RequestSharedPtr;

#endif // __HTTP_REQUEST_H__
