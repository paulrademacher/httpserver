#include "socket.hpp"

Socket::Socket(boost::asio::io_service& io_service) {
  socket_.reset(new tcp::socket(io_service));
}

tcp::socket& Socket::get_raw_socket() {
  return *socket_;
}

size_t Socket::read_until(boost::asio::streambuf& buffer,
    const std::string& delim, boost::system::error_code& error_code) {
  return boost::asio::read_until(*socket_, buffer, std::string("\r\n\r\n"), error_code);
}

size_t Socket::read(boost::asio::streambuf& buffer, boost::system::error_code& error_code) {
  return boost::asio::read(*socket_, buffer, error_code);
}

void Socket::write(const std::string& output, boost::system::error_code& error_code) {
  boost::asio::write(*socket_, boost::asio::buffer(output), error_code);
}

Socket::~Socket() {
  if (socket_) {
    boost::system::error_code ignored_error;
    socket_->shutdown(tcp::socket::shutdown_both, ignored_error);
    socket_->close();
  }
}
