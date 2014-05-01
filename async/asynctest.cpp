#include "async.hpp"
#include <boost/asio.hpp>

void foo(async::SeriesCallback<int> &callback) {
  printf("FOO\n");

  callback(async::OK, 11);
}

// TODO: this signature will compire with or without reference!

void bar(async::SeriesCallback<int> &callback) {
  printf("BAR\n");

  callback(async::OK, 22);
}

void completion(async::ErrorCode error, async::VectorSharedPtr<int> results) {
  printf("--------------------\n");
  printf("size: %lu\n", results->size());
  for (int x : *results) {
    printf("%d\n", x);
  }
}

int main(int argc, char *argv[]) {
  boost::asio::io_service *io_service = new boost::asio::io_service;

  std::vector<async::Task<int>> tasks = {foo, bar};
  async::series<int>(tasks, completion);

  io_service->run();

  return 0;
}
