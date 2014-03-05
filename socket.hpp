#ifndef __HTTP_SOCKET_H__
#define __HTTP_SOCKET_H__

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Socket {
public:
  explicit Socket(boost::asio::io_service& io_service);

  ~Socket();

  size_t read_until(boost::asio::streambuf& buffer,
    const std::string& delim, boost::system::error_code& error_code);

  void write(const std::string& output, boost::system::error_code& error_code);

  tcp::socket& get_raw_socket();

private:
  std::unique_ptr<tcp::socket> socket_;
};

typedef std::shared_ptr<Socket> SocketSharedPtr;

#endif // __HTTP_SOCKET_H__
