#pragma once

#ifndef __ASYNC_ASYNC_HPP__
#define __ASYNC_ASYNC_HPP__

#include <functional>
#include <vector>

namespace async {

typedef enum {
  OK = 0,
  FAIL = -1
} ErrorCode;

template<typename T>
using VectorSharedPtr = std::shared_ptr<std::vector<T>>;

template<typename T>
using SeriesCallback = std::function<void(ErrorCode error, T result)>;

template<typename T>
using SeriesCompletionCallback = std::function<void(ErrorCode, VectorSharedPtr<T>)>;

template <typename T>
using Task = std::function<void(SeriesCallback<T>&)>;

template<typename T>
using TaskVector = std::vector<Task<T>>;

template<typename T>
void noop_series_final_callback(ErrorCode e, VectorSharedPtr<T> p) {};

void TEST(async::SeriesCallback<int> callback) {
  printf("TEST\n");
  callback(async::OK, 11);
}

// `tasks` and `final_callback` are passed by reference.  It is the responsibility of the
// caller to ensure that their lifetime exceeds the lifetime of the series call.
template<typename T>
void series_with_callstack(std::vector<Task<T>> &tasks,
    const std::function<void(ErrorCode, VectorSharedPtr<T>)> &final_callback=noop_series_final_callback<T>) {

  auto results = std::make_shared<std::vector<T>>();

  // If task list is empty, invoke the final callback immediately with a success code and
  // an empty results list.  Doing so now simplifies the iteration below.
  if (tasks.size() == 0) {
    if (final_callback) {
      final_callback(OK, results);
    }
    return;
  }

  using Iterator = typename TaskVector<T>::iterator;

  Iterator task_iter = tasks.begin();
  SeriesCallback<T> callback = [&tasks, task_iter, &final_callback, results, &callback](ErrorCode error, T result) mutable {
    printf("err=%d   iter: %x %x   - &iter=%x  &cb=%x\n", error, task_iter, tasks.end(), &task_iter, &callback);
    results->push_back(result);
    if (error == OK) {
      ++task_iter;
      if (task_iter == tasks.end()) {
        final_callback(error, results);
      } else {
        // `callback` is being passed by reference.  If it were by value, then we'd be
        // creating a copy of callback and all the captured variables, and task_iter would
        // be effectively reset.
        (*task_iter)(callback);
      }
    } else {
      final_callback(error, results);
    }
  };

  (*task_iter)(callback);
}


// `tasks` and `final_callback` are passed by reference.  It is the responsibility of the
// caller to ensure that their lifetime exceeds the lifetime of the series call.
template<typename T>
void series(std::vector<Task<T>> &tasks,
    const std::function<void(ErrorCode, VectorSharedPtr<T>)> &final_callback=noop_series_final_callback<T>) {

  auto results = std::make_shared<std::vector<T>>();

  // If task list is empty, invoke the final callback immediately with a success code and
  // an empty results list.
  if (tasks.size() == 0) {
    final_callback(OK, results);
    return;
  }

  using Iterator = typename TaskVector<T>::iterator;
  auto iter = std::make_shared<Iterator>(tasks.begin());
  auto inside_task = std::make_shared<bool>(false);
  auto last_error = std::make_shared<ErrorCode>(OK);

  SeriesCallback<T> callback = [results, inside_task, last_error, invoke_next_task](ErrorCode error, T result) mutable {
    results->push_back(result);
    *last_error = error;
    if (!inside_task) {
      invoke_next_task();
    }
  };
  auto invoke_next_task = [iter]() {
    ++(*iter);
    invoke_current_task();
  };
  auto invoke_current_task = [inside_task, iter, callback]() mutable {
    *inside_task = true;
    auto task = **iter;
    task(callback);
    *inside_task = false;
  };

  while (*iter != tasks.end()) {
    invoke_current_task();
    ++(*iter);
  }
}

}

#endif
