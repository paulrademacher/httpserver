#include "socket.hpp"

Socket::Socket(boost::asio::io_service& io_service) {
  DEBUG_CTOR("Socket");

  socket_.reset(new tcp::socket(io_service));
}

tcp::socket& Socket::get_raw_socket() {
  return *socket_;
}

void Socket::read_until(boost::asio::streambuf& buffer,
    const std::string& delim, ReadHandler handler) {
  return boost::asio::async_read_until(*socket_, buffer, std::string("\r\n\r\n"), handler);
}

size_t Socket::read(boost::asio::streambuf& buffer, boost::system::error_code& error_code) {
  return boost::asio::read(*socket_, buffer, error_code);
}

void Socket::write(const std::string& output, boost::system::error_code& error_code) {
  boost::asio::write(*socket_, boost::asio::buffer(output), error_code);
}

Socket::~Socket() {
  DEBUG_DTOR("Socket");

  if (socket_) {
    boost::system::error_code ignored_error;
    socket_->shutdown(tcp::socket::shutdown_both, ignored_error);
    socket_->close();
  }
}
