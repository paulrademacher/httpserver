#ifndef __HTTP_COMMON_H__
#define __HTTP_COMMON_H__

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#define DEBUG_LIFECYCLE

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

#endif // __HTTP_COMMON_H__
