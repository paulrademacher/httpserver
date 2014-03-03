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

void Server::run() {
  printf("Starting server on port %d...\n", port_);

  try {
    while (true) {
      SocketSharedPtr socket(new tcp::socket(*io_service_));
      acceptor_->accept(*socket);

      printf("---------------- Connection: --------------------\n");

      TransactionSharedPtr transaction(new Transaction(*this, socket));

      transaction->start();
    }
  } catch (std::exception& e) {
    std::cerr << "EXCEPTION: " << e.what() << std::endl;
  }
}

void Server::add_route(std::string regex_string, RequestHandlerFunction func,
    MethodEnum method) {
  RouteSharedPtr route = std::make_shared<Route>(regex_string, func, method);
  routes_.push_back(route);
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
