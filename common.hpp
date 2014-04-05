#ifndef __HTTP_COMMON_HPP__
#define __HTTP_COMMON_HPP__

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// #define DEBUG_LIFECYCLE

#ifdef DEBUG_LIFECYCLE
#define DEBUG_CTOR(name) { printf("> %s CTOR: %p\n", name, this); }
#define DEBUG_DTOR(name) { printf("< %s DTOR: %p\n", name, this); }
#else
#define DEBUG_CTOR(name)
#define DEBUG_DTOR(name)
#endif

enum MethodEnum {
  METHOD_ANY = 0,
  METHOD_GET = 1,
  METHOD_POST = 2
};

class AsyncMethods;
class Request;
class Response;
class Server;
class Transaction;

typedef std::shared_ptr<Server> ServerSharedPtr;
typedef std::shared_ptr<AsyncMethods> AsyncMethodsSharedPtr;

#include <boost/asio/steady_timer.hpp>
typedef std::shared_ptr<boost::asio::steady_timer> SteadyTimerSharedPtr;

typedef std::function<void()> TimeoutHandler;

#endif // __HTTP_COMMON_HPP__
