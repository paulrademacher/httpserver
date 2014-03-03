#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <string>
#include <set>

#include "common.hpp"
#include "route.hpp"
#include "transaction.hpp"

class Server {
public:
  explicit Server(std::string host, int port);

  void run();
  void add_route(std::string regex_string, RequestHandlerFunction func,
    MethodEnum method=METHOD_GET);
  RouteSharedPtr match_route(std::string &uri, std::string &method_string);

private:
  std::string host_;
  int port_;

  std::unique_ptr<boost::asio::io_service> io_service_;
  std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;

  std::set<TransactionSharedPtr> transactions_;

  std::vector<RouteSharedPtr> routes_;
};

#endif // __HTTP_SERVER_H__
