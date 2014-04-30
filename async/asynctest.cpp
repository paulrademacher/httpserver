#include "async.hpp"
#include <boost/asio.hpp>

void foo(async::SeriesCallback<int> callback) {
  printf("FOO\n");

  callback(async::OK, 11);
}

void bar(async::SeriesCallback<int> callback) {
  printf("BAR\n");

  callback(async::OK, 22);
}

void completion(async::ErrorCode error, async::VectorSharedPtr<int> results) {
  printf("--------------------\n");
  printf("size: %d\n", results->size());
  for (int x : *results) {
    printf("%d\n", x);
  }
}

int main(int argc, char *argv[]) {
  boost::asio::io_service *io_service = new boost::asio::io_service;

  ////  async::series<int>({foo, bar}, completion);
  //      [](unsigned int err, std::vector<int> results) { printf("All done\n"); });

  std::vector<int> foo { 1, 2, 3 };
  auto iter = foo.begin();
  std::function<void(void)> cb = [iter, &cb, foo]() mutable {
    printf(". %d %p   %x %x %x\n", *iter, &iter, iter, foo.begin(), foo.end());
    if (iter != foo.end()) {
      printf("  %d %p %d\n", *iter, &iter, foo.size());
      ++iter;
      cb();
    }
  };
  cb();



  io_service->run();

  return 0;
}
