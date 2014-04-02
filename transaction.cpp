#include <boost/algorithm/string.hpp>

#include "async_methods.hpp"
#include "response.hpp"
#include "server.hpp"
#include "transaction.hpp"

Transaction::Transaction(Server &server, SocketSharedPtr socket,
    boost::asio::io_service &io_service)
  : server_(server), socket_(socket), io_service_(io_service) {

  DEBUG_CTOR("Transaction");
}

void Transaction::start() {
  // TODO: Put a limit on size here.

  // Hold on to this Transaction for lifetime of closure.
  TransactionSharedPtr hold_this = shared_from_this();

  socket_->read_until(read_buffer_, std::string("\r\n\r\n"),
    [&, hold_this](const boost::system::error_code &error, std::size_t bytes_received) mutable {

      // TODO: Does error matter here?

      read_request();
      process_request();
    });
}

void Transaction::read_request() {
  std::vector<std::string> header_lines;
  boost::system::error_code error;

  // read_until may read past the delimeter, resulting in leftover bytes.
  // http://think-async.com/Asio/boost_asio_1_2_0/doc/html/boost_asio/overview/core/line_based.html
  std::string leftover_bytes;

  std::istream stream(&read_buffer_);
  while (!stream.eof()) {
    std::string s;
    std::getline(stream, s);
    if (s[s.length() - 1] == '\r') {
      s = s.substr(0, s.length() - 1);  // Strip CR.
    } else {
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
        boost::asio::read(socket_->get_raw_socket(), boost::asio::buffer(body_bytes));
        if (error) {
          // TODO: handle error.
        }
        body = leftover_bytes + std::string(body_bytes.data(), body_bytes.size());
        // TODO: Check # bytes read.
      }
    }
  }

  if (header_lines.size() > 0) {
    request_ = std::make_shared<Request>(*this, header_lines, body);
    printf("Done reading request\n");
  }

    //    if (error == boost::asio::error::eof) {
    //      break;
    //    }

}

void Transaction::process_request() {
  response_ = std::make_shared<Response>(*this);

  RouteSharedPtr route = server_.match_route(request_->path, request_->method);

  if (route) {
    printf("MATCH: %s\n", request_->path.c_str());

    async_methods_ = std::make_shared<AsyncMethods>(*this);

    // Bracket the handler call with begin_op/end_op, which bracket each async call, but
    // here ensure that the same code path is followed [end_op()] even when no async calls
    // are triggered in the request handler.
    async_methods_->begin_op();

    route->call(request_, response_, async_methods_);

    async_methods_->end_op();
  } else {
    printf("NO MATCH: %s\n", request_->path.c_str());

    response_->set_status_code(404);

    // response will fall out of scope and deallocate (send itself) immediately.
  }
}

void Transaction::send_response() {
  std::string output = response_->get_output();

  if (!output.length()) {
    // Use default response HTML if nothing was set.
    output = server_.response_code_.get_response_html(response_->get_status_code());
  }

  std::cout << "ST: " << response_->get_status_code() << std::endl;

  std::string& status_string = server_.response_code_.get_status_string(response_->get_status_code());

  boost::system::error_code ignored_error;
  socket_->write("HTTP/1.1 " + status_string + "\r\n", ignored_error);
  socket_->write("Content-Type: text/html\r\n", ignored_error);
  socket_->write("Content-Length: " + std::to_string(output.size()) + "\r\n", ignored_error);
  socket_->write("\r\n", ignored_error);
  socket_->write(output, ignored_error);
}

void Transaction::notify_no_pending_ops() {
  send_response();

  server_.notify_transaction_finished(shared_from_this());
}

Transaction::~Transaction() {
  DEBUG_DTOR("Transaction");
}
