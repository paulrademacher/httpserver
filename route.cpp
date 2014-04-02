#include "route.hpp"

Route::Route(Server &server, std::string regex_string, RequestHandlerFunction func, MethodEnum method)
  : regex_(boost::regex(regex_string)), func_(func), method_(method), server_(server) {
  DEBUG_CTOR("Route");
}

Route::~Route() {
  DEBUG_DTOR("Route");
}

bool Route::matches(std::string &uri, MethodEnum method) {
  if (boost::regex_match(uri, regex_)) {
    if (method_ == METHOD_ANY || method_ == method) {
      return true;
    }
  }
  return false;
}

bool Route::call(Request &request, Response &response, AsyncMethods &async_methods) {
  if (func_) {
    func_(request, response, async_methods);
    return true;
  } else {
    // TODO: Handle error.
    printf("ERROR: No function for route\n");
  }
  return false;
}
