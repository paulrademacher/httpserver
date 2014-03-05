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
  RequestSharedPtr request = std::make_shared<Request>();

  while (true) {
    boost::system::error_code error;
    boost::asio::streambuf buffer;
    size_t len = socket_->read_until(buffer, std::string("\r\n\r\n"), error);

    std::istream stream(&buffer);

    // TODO: Deal with malformed request (e.g., not 3 parts).
    // TODO: Deal with too-long uri or header line.

    stream >> method >> uri >> http_version;
    request->method = method;
    request->uri = uri;
    request->http_version = http_version;

    bool is_first_line = true;  // TODO: fix this ugliness.  It's because I'm mixing >>
                                // and getline.
    bool done = false;

    while (!stream.eof()) {
      std::string s;
      std::getline(stream, s);

      if (is_first_line) {
        is_first_line = false;
        continue;
      }

      if (s.length() < 2) {
        done = true;
        break;
      }
      input_lines_.push_back(s);
    }

    if (done) {
      break;
    }

    if (error == boost::asio::error::eof) {
      break;
    }
  }

  return request;
}

void Transaction::process_request(RequestSharedPtr &request) {
  for (auto line : input_lines_) {
    std::cout << line << std::endl;
  }

  RouteSharedPtr route = server_.match_route(request->uri, request->method);

  ResponseSharedPtr response = std::make_shared<Response>();
  if (route) {
    printf("MATCH: %s\n", request->uri.c_str());
    route->call(request, response);
    send_response(response);
  } else {
    printf("NO MATCH: %s\n", request->uri.c_str());

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

