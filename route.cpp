#include "route.hpp"

Route::Route(std::string regex_string, RequestHandlerFunction func, MethodEnum method)
  : regex_(boost::regex(regex_string)), func_(func), method_(method) {
}

bool Route::matches(std::string &uri, MethodEnum method) {
  if (boost::regex_match(uri, regex_)) {
    if (method_ == METHOD_ANY || method_ == method) {
      return true;
    }
  }
  return false;
}

bool Route::call(RequestSharedPtr request, ResponseSharedPtr response) {
  if (func_) {
    func_(*request, *response);
    return true;
  } else {
    // TODO: Handle error.
    printf("ERROR: No function for route\n");
  }
  return false;
}
