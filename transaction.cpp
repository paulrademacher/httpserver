#include "response.hpp"
#include "server.hpp"
#include "transaction.hpp"

Transaction::Transaction(Server& server, SocketSharedPtr &socket)
  : socket_(socket), server_(server) {
}

void Transaction::start() {
  RequestSharedPtr request = read_request();
  process_request(request);
}

RequestSharedPtr Transaction::read_request() {
  std::string method;
  std::string uri;
  std::string http_version;
  std::vector<std::string> header_lines;
  boost::system::error_code error;
  boost::asio::streambuf buffer;
  size_t len;

  // TODO: Put a limit on size here.
  len = socket_->read_until(buffer, std::string("\r\n\r\n"), error);

  std::istream stream(&buffer);
  while (!stream.eof()) {
    std::string s;
    std::getline(stream, s);
    header_lines.push_back(s);
  }

  for (auto line : header_lines) {
    std::cout << line << std::endl;
  }

  //  len = socket_->read(buffer, error);

  RequestSharedPtr request = std::make_shared<Request>(header_lines, "");

    //    if (error == boost::asio::error::eof) {
    //      break;
    //    }

  return request;
}

void Transaction::process_request(RequestSharedPtr &request) {
  RouteSharedPtr route = server_.match_route(request->path, request->method);

  ResponseSharedPtr response = std::make_shared<Response>();
  if (route) {
    printf("MATCH: %s\n", request->path.c_str());
    route->call(request, response);
    send_response(response);
  } else {
    printf("NO MATCH: %s\n", request->path.c_str());

    response->set_status_code(404);
    send_response(response);
  }
}

void Transaction::send_response(ResponseSharedPtr &response) {
  std::string output = response->get();

  if (!output.length()) {
    // Use default response HTML if nothing was set.
    output = server_.response_code_.get_response_html(response->get_status_code());
  }

  std::cout << "ST: " << response->get_status_code() << std::endl;

  std::string& status_string = server_.response_code_.get_status_string(response->get_status_code());

  boost::system::error_code ignored_error;
  socket_->write("HTTP/1.1 " + status_string + "\r\n", ignored_error);
  socket_->write("Content-Type: text/html\r\n", ignored_error);
  socket_->write("Content-Length: " + std::to_string(output.size()) + "\r\n", ignored_error);
  socket_->write("\r\n", ignored_error);
  socket_->write(output, ignored_error);

}

