#ifndef __HTTP_TRANSACTION_H__
#define __HTTP_TRANSACTION_H__

#include <string>
#include <vector>

#include "common.hpp"
#include "request.hpp"
#include "socket.hpp"

using boost::asio::ip::tcp;

class Server;
class Response;

class Transaction : public std::enable_shared_from_this<Transaction> {
public:
  explicit Transaction(Server &server, SocketSharedPtr &socket_);

  ~Transaction();

  void start();
private:
  SocketSharedPtr socket_;
  Server &server_;

  RequestSharedPtr read_request();
  void process_request(RequestSharedPtr &request);
  void send_response(Response &response);

  friend Response;  // TODO: Is this right OO layout?
};

typedef std::shared_ptr<Transaction> TransactionSharedPtr;

#endif // __HTTP_TRANSACTION_H__
