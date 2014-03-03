#ifndef __HTTP_ROUTE_H__
#define __HTTP_ROUTE_H__

#include <string>
#include <boost/regex.hpp>

#include "common.hpp"
#include "request_handler.hpp"
#include "response.hpp"

class Route {
public:
  explicit Route(std::string regex_string, RequestHandlerFunction func,
    MethodEnum method=METHOD_ANY);

  bool matches(std::string &uri, MethodEnum method);
  bool call(RequestSharedPtr request, ResponseSharedPtr response);

  // TODO: Clean up SharedPtr& vs by-val.
private:
  boost::regex regex_;
  MethodEnum method_;
  RequestHandlerFunction func_;
};

typedef std::shared_ptr<Route> RouteSharedPtr;

#endif // __HTTP_ROUTE_H__
