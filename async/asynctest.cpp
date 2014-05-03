#include "async.hpp"

void task1(async::SeriesCallback<int> &callback) {
  callback(async::OK, 1);
}

// TODO: this signature will compile with or without reference!

void task2(async::SeriesCallback<int> &callback) {
  callback(async::OK, 2);
}

void task3(async::SeriesCallback<int> &callback) {
  callback(async::OK, 3);
}

void task4(async::SeriesCallback<int> &callback) {
  callback(async::OK, 4);
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

  std::vector<async::Task<int>> tasks = { task1, task2, task3, task4};
  async::series<int>(tasks, completion_callback);

  assert(async::debug_series_state_count == 0);
}

void test2() {
  assert(async::debug_series_state_count == 0);

  async::SeriesCallback<int> *deferred_callback;

  auto task_deferred_initiate = [&deferred_callback](async::SeriesCallback<int> &callback) {
    deferred_callback = &callback;
  };

  auto task_deferred_complete = [&deferred_callback]() {
    (*deferred_callback)(async::OK, 99);
  };

  std::vector<async::Task<int>> tasks = { task1, task2, task_deferred_initiate, task3, task4};
  async::series<int>(tasks, completion_callback);

  task_deferred_complete();

  assert(async::debug_series_state_count == 0);
}

void test3() {
  assert(async::debug_series_state_count == 0);

  async::SeriesCallback<int> *deferred_callback;

  auto task_deferred_initiate = [&deferred_callback](async::SeriesCallback<int> &callback) {
    deferred_callback = &callback;
  };

  auto task_deferred_complete = [&deferred_callback]() {
    (*deferred_callback)(async::OK, 99);
  };

  std::vector<async::Task<int>> tasks = { task_deferred_initiate, task_deferred_initiate, task_deferred_initiate};
  async::series<int>(tasks, completion_callback);

  task_deferred_complete();
  task_deferred_complete();
  task_deferred_complete();

  assert(async::debug_series_state_count == 0);
}

int main(int argc, char *argv[]) {
  test1();
  test2();
  test3();

  return 0;
}
