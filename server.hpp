#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include <string>
#include <set>

#include <boost/asio/steady_timer.hpp>

#include "common.hpp"
#include "response_code.hpp"
#include "route.hpp"
#include "route_ptr.hpp"
#include "transaction.hpp"

typedef std::function<void()> TimeoutHandler;
typedef std::shared_ptr<boost::asio::steady_timer> SteadyTimerSharedPtr;

class Server {
public:
  explicit Server(std::string host, int port);

  ~Server() { std::cout << "Server dtor" << std::endl; }

  void run();

  RoutePtr route(std::string regex_string, MethodEnum method=METHOD_GET,
    RequestHandlerFunction func=nullptr);

  RouteSharedPtr match_route(std::string &uri, std::string &method_string);

  ResponseCode response_code_;

  void async_wait(TimeoutHandler handler, unsigned int timeout_ms);

private:
  void async_accept();

  std::string host_;
  int port_;

  std::set<SteadyTimerSharedPtr> steady_timers_;
  std::unique_ptr<boost::asio::io_service> io_service_;
  std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;

  std::set<TransactionSharedPtr> transactions_;

  std::vector<RouteSharedPtr> routes_;
};

#endif // __HTTP_SERVER_H__
