#ifndef __HTTP_REQUEST_HANDLER_H__
#define __HTTP_REQUEST_HANDLER_H__

#include "common.hpp"
#include "request.hpp"
#include "response.hpp"

class RequestHandler {
public:
};

typedef void (*RequestHandlerFunction)(RequestSharedPtr request, ResponseSharedPtr response);

#endif // __HTTP_REQUEST_HANDLER_H__
