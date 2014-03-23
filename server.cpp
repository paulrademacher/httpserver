#include <stdio.h>
#include <string>
#include <iostream>

#include <boost/array.hpp>

#include "server.hpp"

using boost::asio::ip::tcp;

Server::Server(std::string host, int port) {
  host_ = host;
  port_ = port;

  io_service_.reset(new boost::asio::io_service);
  acceptor_.reset(new tcp::acceptor(*io_service_, tcp::endpoint(tcp::v4(), port_)));
}

void ghandler(const boost::system::error_code& error) {
  std::cout << "Handler: " << error.message() << std::endl;
}

void Server::run() {
  std::cout << "Starting server on port " << port_ << "..." << std::endl;

  async_accept();

  io_service_->run();
}

void Server::async_accept() {
  try {
    SocketSharedPtr socket(new Socket(*io_service_));

    acceptor_->async_accept(socket->get_raw_socket(),
      [=](const boost::system::error_code &error) mutable {

        printf("---------------- Connection: --------------------\n");

        TransactionSharedPtr transaction(new Transaction(*this, socket));
        transaction->start();

        async_accept();
      });
  } catch (std::exception& e) {
    std::cerr << "EXCEPTION in async_accept: " << e.what() << std::endl;
  }
}

RoutePtr Server::route(std::string regex_string, MethodEnum method,
    RequestHandlerFunction func) {
  RouteSharedPtr route = std::make_shared<Route>(this, regex_string, func, method);
  routes_.push_back(route);
  RoutePtr route_ptr(route);
  return route_ptr;
}

RouteSharedPtr Server::match_route(std::string &uri, std::string &method_string) {
  MethodEnum method;
  if (method_string == "GET") {
    method = METHOD_GET;
  } else if (method_string == "POST") {
    method = METHOD_POST;
  }

  for (auto it = routes_.begin(); it < routes_.end(); ++it) {
    if ((*it)->matches(uri, method)) {
      return *it;
    }
  }
  return nullptr;
}

void Server::async_wait(TimeoutHandler handler, unsigned int timeout_ms) {
  // http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference/steady_timer.html

  SteadyTimerSharedPtr timer = std::make_shared<boost::asio::steady_timer>(*io_service_,
      std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms));

  // Hold on to timer so it doesn't go out of scope and cancel itself.
  steady_timers_.insert(timer);

  timer->async_wait([=] (const boost::system::error_code& error) {
      std::cout << "handler: " << error.message() << std::endl;
      if (!error) {
        // Timer expired.
        handler();
      }
      steady_timers_.erase(timer);
    });
}
