#include "response.hpp"
#include "transaction.hpp"

Response::Response(Transaction &transaction)
    : status_code_(200), transaction_(transaction) {
  DEBUG_CTOR("Response");
}

void Response::write(std::string &str) {
  stream_ << str;
}

void Response::write(std::string str) {
  stream_ << str;
}

std::string Response::get_output() {
  return stream_.str();
}

void Response::set_status_code(int status_code) {
  status_code_ = status_code;
}

int Response::get_status_code() {
  return status_code_;
}

Response::~Response() {
  DEBUG_DTOR("Response");
}
