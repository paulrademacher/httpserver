#pragma once

#include <string>
#include <vector>

#include <boost/asio.hpp>

class AsyncHttpClient {
public:
  AsyncHttpClient(std::shared_ptr<boost::asio::io_service> io_service,
    const std::string &uri,
    const std::string &method="GET",
    const std::vector<std::string> &headers=std::vector<std::string>(),
    const std::string &body="");
  ~AsyncHttpClient();

  bool connect();

  AsyncHttpClient(const AsyncHttpClient&) = delete;
  AsyncHttpClient& operator=(const AsyncHttpClient&) = delete;

private:
  std::shared_ptr<boost::asio::io_service> io_service_;
  std::string uri_;
  std::string method_;
  std::vector<std::string> headers_;
  std::string body_;
};
