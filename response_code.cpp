#include "response_code.hpp"

ResponseCode::ResponseCode() {
  status_string_ = {
    { 200, "200 OK" },
    { 301, "301 Moved Permanently" },
    { 404, "404 Not Found" }
  };

  response_html_ = {
    { 200, "" },
    { 301, "" },
    { 404, "OOPS" }
    };
}

void ResponseCode::set_response_html(int code, std::string& html) {
  response_html_.at(code) = html;
}

std::string& ResponseCode::get_status_string(int code) {
  return status_string_.at(code);
}

std::string& ResponseCode::get_response_html(int code) {
  return response_html_.at(code);
}
