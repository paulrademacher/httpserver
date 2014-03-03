#ifndef __HTTP_RESPONSE_CODE_H__
#define __HTTP_RESPONSE_CODE_H__

#include <map>
#include <string>

class ResponseCode {
public:
  ResponseCode();

  std::string& get_status_string(int code);
  std::string& get_response_html(int code);

  void set_response_html(int code, std::string& html);

private:
  std::map<int, std::string> status_string_;
  std::map<int, std::string> response_html_;
};

#endif // __HTTP_RESPONSE_CODE_H__
