#ifndef __HTTP_SERVER_HPP__
#define __HTTP_SERVER_HPP__

#include <string>
#include <unordered_set>

#include "common.hpp"
#include "response_code.hpp"
#include "route.hpp"
#include "route_ptr.hpp"
#include "transaction.hpp"

class Server : public std::enable_shared_from_this<Server> {
public:
  explicit Server(std::string host, int port);

  ~Server() { std::cout << "Server dtor" << std::endl; }

  void run();

  RoutePtr route(std::string regex_string, MethodEnum method=METHOD_GET,
    RequestHandlerFunction func=nullptr);

  RouteSharedPtr match_route(std::string &uri, std::string &method_string);

  ResponseCode response_code_;

  boost::asio::io_service &get_io_service() { return *io_service_; }

private:
  void async_accept();

  std::string host_;
  int port_;

  std::unique_ptr<boost::asio::io_service> io_service_;
  std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
  std::unordered_set<TransactionSharedPtr> transactions_;
  std::vector<RouteSharedPtr> routes_;

  friend Transaction;
  void notify_transaction_finished(TransactionSharedPtr transaction_);
};

#endif // __HTTP_SERVER_HPP__
