#ifndef __HTTP_ROUTE_HPP__
#define __HTTP_ROUTE_HPP__

#include <string>
#include <boost/regex.hpp>

#include "async_methods.hpp"
#include "common.hpp"
#include "request_handler.hpp"
#include "response.hpp"

class Server;
class RoutePtr;

class Route {
public:
  explicit Route(Server &server, std::string regex_string, RequestHandlerFunction func,
      MethodEnum method=METHOD_ANY);
  ~Route();

  bool matches(std::string &uri, MethodEnum method);
  bool call(Request &request, Response &response, AsyncMethods &async_methods);

  // TODO: Clean up SharedPtr& vs by-val.

  Route(const Route&) = delete;
  Route& operator=(const Route&) = delete;
private:
  boost::regex regex_;
  MethodEnum method_;
  RequestHandlerFunction func_;
  Server &server_;  // Owner.

  friend RoutePtr;
};

typedef std::shared_ptr<Route> RouteSharedPtr;

#endif // __HTTP_ROUTE_HPP__
