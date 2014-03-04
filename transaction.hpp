#ifndef __HTTP_TRANSACTION_H__
#define __HTTP_TRANSACTION_H__

#include <string>
#include <vector>

#include "common.hpp"
#include "request.hpp"
#include "socket.hpp"

using boost::asio::ip::tcp;

class Server;

class Transaction {
public:
  explicit Transaction(Server &server, SocketSharedPtr &socket_);

  void start();
private:
  SocketSharedPtr socket_;
  Server &server_;

  std::vector<std::string> input_lines_;

  RequestSharedPtr read_request();
  void process_request(RequestSharedPtr &request);
  void send_response(ResponseSharedPtr &response);
};

typedef std::shared_ptr<Transaction> TransactionSharedPtr;

#endif // __HTTP_TRANSACTION_H__
