#include "response.hpp"

void Response::write(std::string &str) {
  stream_ << str;
}

void Response::write(std::string str) {
  stream_ << str;
}

std::string Response::get() {
  return stream_.str();
}
