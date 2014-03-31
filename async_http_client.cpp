#include "async_http_client.hpp"

AsyncHttpClient::AsyncHttpClient(std::shared_ptr<boost::asio::io_service> io_service,
  const std::string &uri,
  const std::string &method,
  const std::vector<std::string> &headers,
  const std::string &body) 
  : io_service_(io_service), uri_(uri), method_(method), headers_(headers), body_(body) {
}
