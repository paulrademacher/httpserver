#ifndef __HTTP_RESPONSE_H__
#define __HTTP_RESPONSE_H__

#include <string>
#include <sstream>

class Response {
public:
  void write(std::string &str);
  void write(std::string str);

  std::string get();
private:
  std::stringstream stream_;
};

typedef std::shared_ptr<Response> ResponseSharedPtr;

#endif // __HTTP_RESPONSE_H__
