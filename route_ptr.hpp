#ifndef __HTTP_ROUTE_PTR_H__
#define __HTTP_ROUTE_PTR_H__

#include "route.hpp"

class RoutePtr {
public:
  RoutePtr(RouteSharedPtr route_shared_ptr) : route_shared_ptr_(route_shared_ptr) {};

  RoutePtr &operator>>(RequestHandlerFunction func) {
    route_shared_ptr_->func_ = func;
    return *this;
  }

private:
  RouteSharedPtr route_shared_ptr_;
};

#endif // __HTTP_ROUTE_PTR_H__
