#include "async.hpp"

void task1(async::SeriesCallback<int> &callback) {
  callback(async::OK, 1);
}

// TODO: this signature will compile with or without reference!

void task2(async::SeriesCallback<int> &callback) {
  callback(async::OK, 2);
}

async::SeriesCallback<int> deferred_callback;

void task3_deferred_initiate(async::SeriesCallback<int> &callback) {
  deferred_callback = callback;
}

void task3_deferred_complete() {
  deferred_callback(async::OK, 3);
}

void task4(async::SeriesCallback<int> &callback) {
  callback(async::OK, 4);
}

void task5(async::SeriesCallback<int> &callback) {
  callback(async::OK, 5);
}

void completion_callback(async::ErrorCode error, std::vector<int> &results) {
  printf("--------------------\n");
  for (int x : results) {
    printf("%d ", x);
  }
  printf("\n");
}

void test1() {
  assert(async::debug_series_state_count == 0);

  //  std::vector<async::Task<int>> tasks = { task1 }; //{ task3_deferred_initiate }; //x{task1, task2, task3_deferred_initiate, task4, task5};
  std::vector<async::Task<int>> tasks = { task1, task2, task3_deferred_initiate, task4, task5, task3_deferred_initiate};
  async::series<int>(tasks, completion_callback);

  task3_deferred_complete();
  task3_deferred_complete();

  assert(async::debug_series_state_count == 0);
}

void test2() {
  assert(async::debug_series_state_count == 0);

  std::vector<async::Task<int>> tasks = { task1, task2, task4, task5};
  async::series<int>(tasks, completion_callback);

  assert(async::debug_series_state_count == 0);
}

void test3() {
  assert(async::debug_series_state_count == 0);

  // ...

  assert(async::debug_series_state_count == 0);
}

int main(int argc, char *argv[]) {
  test1();
  test2();
  test3();

  return 0;
}
