#ifndef __HTTP_RESPONSE_HPP__
#define __HTTP_RESPONSE_HPP__

#include <string>
#include <sstream>

#include "common.hpp"

class Response {
public:
  Response(Transaction &transaction);
  ~Response();

  void write(std::string &str);
  void write(std::string str);

  // Write using stream interface.
  template<typename T> Response& operator<<(T item) {
    stream_ << item;
    return *this;
  }

  std::string get_output();

  void set_status_code(int status_code);

  int get_status_code();

  Response(const Response&) = delete;
  Response& operator=(const Response&) = delete;
private:
  Transaction &transaction_;  // Owner.

  std::stringstream stream_;
  int status_code_;
};

typedef std::shared_ptr<Response> ResponseSharedPtr;

#endif // __HTTP_RESPONSE_HPP__
