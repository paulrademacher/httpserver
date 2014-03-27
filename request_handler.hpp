#ifndef __HTTP_REQUEST_HANDLER_HPP__
#define __HTTP_REQUEST_HANDLER_HPP__

#include "async_methods.hpp"
#include "common.hpp"
#include "request.hpp"
#include "response.hpp"

typedef void (*RequestHandlerFunction)(RequestSharedPtr request,
    ResponseSharedPtr response, AsyncMethodsSharedPtr async_methods);

#endif // __HTTP_REQUEST_HANDLER_HPP__
