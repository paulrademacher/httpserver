#ifndef __HTTP_TRANSACTION_HPP__
#define __HTTP_TRANSACTION_HPP__

#include <string>
#include <vector>

#include "common.hpp"
#include "request.hpp"
#include "response.hpp"
#include "socket.hpp"

using boost::asio::ip::tcp;

class Server;

class Transaction : public std::enable_shared_from_this<Transaction> {
public:
  explicit Transaction(Server &server, SocketSharedPtr socket,
                       boost::asio::io_service &io_service);

  ~Transaction();

  void start();

  Transaction(const Transaction&) = delete;
  Transaction& operator=(const Transaction&) = delete;

private:
  SocketSharedPtr socket_;  // Ownership transfered here.
  Server &server_;  // Owner.
  RequestSharedPtr request_;  // Owned.
  ResponseSharedPtr response_;  // Owned.
  AsyncMethodsSharedPtr async_methods_;  // Owned.

  boost::asio::streambuf read_buffer_;

  boost::asio::io_service &io_service_;

  void read_request();
  void process_request();
  void send_response();

  void notify_no_pending_ops();  // Message from AsyncMethods.

  friend Request;
  friend Response;
  friend AsyncMethods;
};

typedef std::shared_ptr<Transaction> TransactionSharedPtr;

#endif // __HTTP_TRANSACTION_HPP__
