#ifndef __HTTP_RESPONSE_CODE_H__
#define __HTTP_RESPONSE_CODE_H__

#include <map>

class ResponseCode {
public:
  static std::string& get_as_string(int code);

private:
  static std::map<std::string, std::string> code_to_string_;
};

#endif // __HTTP_RESPONSE_CODE_H__
