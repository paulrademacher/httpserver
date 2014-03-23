#include <boost/algorithm/string.hpp>

#include "response.hpp"
#include "server.hpp"
#include "transaction.hpp"

Transaction::Transaction(Server& server, SocketSharedPtr &socket)
  : socket_(socket), server_(server) {

  DEBUG_CTOR("Transaction");
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
  printf("Reading from socket @%p\n", *socket_);
  len = socket_->read_until(buffer, std::string("\r\n\r\n"), error);

  // read_until may read past the delimeter, resulting in leftover bytes.
  // http://think-async.com/Asio/boost_asio_1_2_0/doc/html/boost_asio/overview/core/line_based.html
  std::string leftover_bytes;

  std::istream stream(&buffer);
  while (!stream.eof()) {
    std::string s;
    std::getline(stream, s);
    if (s[s.length() - 1] == '\r') {
      s = s.substr(0, s.length() - 1);  // Strip CR.
    } else {
      printf("Here?\n");
      printf("ERROR: [%s]\n", s.c_str());
    }

    if (s.length() > 0) {
      header_lines.push_back(s);
    } else {
      // Done reading headers.  Slurp any leftover data.
      while (!stream.eof()) {
        std::string leftover_s;
        std::getline(stream, leftover_s);
        leftover_bytes += leftover_s;
      }
    }
  }

  for (auto line : header_lines) {
    //std::cout << "header: [" << line << "]" << std::endl;
  }

  std::string body;
  for (int i = 1; i < header_lines.size(); i++) {
    std::string header = header_lines[i];
    boost::algorithm::to_lower(header);
    std::cout << header << std::endl;
    if (boost::algorithm::starts_with(header, "content-length: ")) {
      int content_length = std::stoi(header.substr(header.find(' ') + 1));
      // TODO: Max content length.
      if (content_length > 0) {
        // We may have leftover bytes from read_until operation.

        std::vector<char> body_bytes(content_length - leftover_bytes.length());
        len = boost::asio::read(socket_->get_raw_socket(), boost::asio::buffer(body_bytes));
        if (error) {
          // TODO: handle error.
        }
        body = leftover_bytes + std::string(body_bytes.data(), body_bytes.size());
        // TODO: Check # bytes read.
      }
    }
  }

  RequestSharedPtr request = std::make_shared<Request>(header_lines, body);

    //    if (error == boost::asio::error::eof) {
    //      break;
    //    }

  printf("Done reading request\n");

  return request;
}

void Transaction::process_request(RequestSharedPtr &request) {
  RouteSharedPtr route = server_.match_route(request->path, request->method);

  ResponseSharedPtr response = std::make_shared<Response>(shared_from_this());
  if (route) {
    printf("MATCH: %s\n", request->path.c_str());
    route->call(request, response);
  } else {
    printf("NO MATCH: %s\n", request->path.c_str());

    response->set_status_code(404);

    // response will fall out of scope and deallocate (send itself) immediately.
  }
}

void Transaction::send_response(Response &response) {
  std::string output = response.get();

  if (!output.length()) {
    // Use default response HTML if nothing was set.
    output = server_.response_code_.get_response_html(response.get_status_code());
  }

  std::cout << "ST: " << response.get_status_code() << std::endl;

  std::string& status_string = server_.response_code_.get_status_string(response.get_status_code());

  boost::system::error_code ignored_error;
  socket_->write("HTTP/1.1 " + status_string + "\r\n", ignored_error);
  socket_->write("Content-Type: text/html\r\n", ignored_error);
  socket_->write("Content-Length: " + std::to_string(output.size()) + "\r\n", ignored_error);
  socket_->write("\r\n", ignored_error);
  socket_->write(output, ignored_error);
}

Transaction::~Transaction() {
  DEBUG_DTOR("Transaction");
}
