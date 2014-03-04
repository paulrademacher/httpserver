#ifndef __HTTP_COMMON_H__
#define __HTTP_COMMON_H__

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum MethodEnum {
  METHOD_ANY = 0,
  METHOD_GET = 1,
  METHOD_POST = 2
};

#endif // __HTTP_COMMON_H__
