#include "async.hpp"
#include <boost/asio.hpp>

void task1(async::SeriesCallback<int> &callback) {
  printf("TASK1\n");

  callback(async::OK, 11);
}

// TODO: this signature will compire with or without reference!

void task2(async::SeriesCallback<int> &callback) {
  printf("TASK2\n");

  callback(async::OK, 22);
}

async::SeriesCallback<int> task3_callback;

void task3_initiate(async::SeriesCallback<int> &callback) {
  printf("TASK3_initiate\n");
  task3_callback = callback;
}

void task3_complete() {
  printf("TASK3_complete\n");

  task3_callback(async::OK, 33);
}

void task4(async::SeriesCallback<int> &callback) {
  printf("TASK4\n");

  callback(async::OK, 44);
}

void task5(async::SeriesCallback<int> &callback) {
  printf("TASK5\n");

  callback(async::OK, 55);
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

  std::vector<async::Task<int>> tasks = {task1, task2, task3_initiate, task4, task5};
  async::series<int>(tasks, completion);

  task3_complete();

  io_service->run();

  return 0;
}
