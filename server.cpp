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

        TransactionSharedPtr transaction(new Transaction(*this, socket, *io_service_));
        transactions_.insert(transaction);  // TODO: Release this.
        transaction->start();

        async_accept();
      });
  } catch (std::exception& e) {
    std::cerr << "EXCEPTION in async_accept: " << e.what() << std::endl;
  }
}

RoutePtr Server::route(std::string regex_string, MethodEnum method,
    RequestHandlerFunction func) {
  RouteSharedPtr route = std::make_shared<Route>(*this, regex_string, func, method);
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

void Server::notify_transaction_finished(TransactionSharedPtr transaction) {
  printf("Server received transaction finished\n");
  printf("Count pre : %lu\n", transactions_.size());
  transactions_.erase(transaction);
  printf("Count post : %lu\n", transactions_.size());
}
