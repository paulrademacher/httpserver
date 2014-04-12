#include "utils.hpp"

namespace utils {

StrVectorSharedPtr split_string(const std::string &string, char delim) {
  StrVectorSharedPtr items = std::make_shared<std::vector<std::string>>();
  std::string item;
  std::stringstream stream(string);

  while (std::getline(stream, item, delim)) {
    items->push_back(item);
  }

  return items;
}

std::string substr(const std::string &string, size_t pos, size_t len) {
  if (pos == std::string::npos) {
    return std::string("");
  }

  try {
    return string.substr(pos, len);
  } catch (const std::out_of_range& e) {
    return std::string("");
  }
}

}
