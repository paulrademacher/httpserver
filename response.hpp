#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <string>
#include <sstream>

class Response {
public:
  Response();

  void write(std::string &str);
  void write(std::string str);

  // Write using stream interface.
  template<typename T> Response& operator<<(T item) {
    stream_ << item;
    return *this;
  }

  std::string get();

  void set_status_code(int status_code);

  int get_status_code();
private:
  std::stringstream stream_;
  int status_code_;
};

typedef std::shared_ptr<Response> ResponseSharedPtr;

#endif // __HTTP_RESPONSE_H__
