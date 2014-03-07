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
  std::map<std::string, std::string> query_params;
  std::map<std::string, std::vector<std::string>> query_params_multi;

  Request(std::vector<std::string>& header_lines, std::string content);
};

typedef std::shared_ptr<Request> RequestSharedPtr;

#endif // __HTTP_REQUEST_H__
