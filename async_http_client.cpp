#include <boost/algorithm/string.hpp>

#include "async_http_client.hpp"

#include "utils.hpp"

// Based on boost_lib/boost_1_55_0/doc/html/boost_asio/example/cpp03/http/client/async_client.cpp
//     by Christopher M. Kohlhoff (chris at kohlhoff dot com)

namespace http_client {

AsyncHttpClient::AsyncHttpClient(boost::asio::io_service& io_service,
    const std::string& uri,
    const std::string& method,
    const std::vector<std::string>& headers,
    const std::string& body)
  : io_service_(io_service),
    resolver_(io_service),
    socket_(io_service),
    uri_(uri),
    method_(method),
    headers_(headers),
    body_(body) {

  std::cout << "----------------------------------------" << std::endl;
  std::cout << uri << std::endl;

  bool is_http = true;
  std::string uri_without_protocol;

  if (boost::algorithm::starts_with(uri_, "http://")) {
    uri_without_protocol = utils::substr(uri_, 7);
  } else if (boost::algorithm::starts_with(uri_, "https://")) {
    uri_without_protocol = utils::substr(uri_, 8);
    is_http = false;
  } else {
    throw MalformedUriException();
  }

  if (uri_without_protocol.length() == 0) {
    throw MalformedUriException();
  }

  size_t path_start = uri_without_protocol.find_first_of("/");
  std::string server_and_port = utils::substr(uri_without_protocol, 0, path_start);
  std::string path = path_start != std::string::npos ?
      utils::substr(uri_without_protocol, path_start) : "/";

  if (server_and_port.length() == 0) {
    throw MalformedUriException();
  }

  int colon = server_and_port.find_first_of(":");
  std::string server = utils::substr(server_and_port, 0, colon);

  std::string port_string;
  if (colon != std::string::npos) {
    port_string = utils::substr(server_and_port, colon + 1);
  }
  int port = port_string.length() > 0 ?
      std::stoi(port_string) : (is_http ? 80 : 443);

  std::cout << "is_http: " << is_http << std::endl;
  std::cout << "server: " << server << std::endl;
  std::cout << "port: " << port << std::endl;
  std::cout << "path: " << path << std::endl;


  // Form the request.  We specift the "Connection: close" header so that the
  // server will close the socket after transmitting the response.  This will
  // allow us to treat all data up until the EOF as the content.

  std::ostream request_stream(&request_);  // Init the member boost::asio::streambuf.
  request_stream << "GET " << path << " HTTP 1.0\r\n";
  request_stream << "Host: " << server << "\r\n";
  request_stream << "Accept: */*\r\n";
  request_stream << "Connection: close\r\n\r\n";
}

AsyncHttpClient::~AsyncHttpClient() {
}

void AsyncHttpClient::fetch(std::function<std::string()> callback) {
}

}
