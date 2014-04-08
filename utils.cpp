#include "utils.hpp"

StrVectorSharedPtr split_string(const std::string &string, char delim) {
  StrVectorSharedPtr items = std::make_shared<std::vector<std::string>>();
  std::string item;
  std::stringstream stream(string);

  while (std::getline(stream, item, delim)) {
    items->push_back(item);
  }

  return items;
}
