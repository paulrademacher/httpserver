#pragma once

#ifndef __ASYNC_ASYNC_HPP__
#define __ASYNC_ASYNC_HPP__

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace async {

typedef enum {
  OK = 0,
  FAIL = -1
} ErrorCode;

class DebugScope {
public:
  std::string name_;
  DebugScope(std::string name) : name_(name) { std::cout << "> scope:" << name_ << std::endl; };
  ~DebugScope() { std::cout << "< scope:" << name_ << std::endl; };
};

template<typename T>
using VectorSharedPtr = std::shared_ptr<std::vector<T>>;

template<typename T>
using SeriesCallback = std::function<void(ErrorCode error, T result)>;

template<typename T>
void noop_series_callback(ErrorCode e, T result) {};

template<typename T>
using SeriesCompletionCallback = std::function<void(ErrorCode, VectorSharedPtr<T>)>;

template<typename T>
void noop_series_final_callback(ErrorCode e, VectorSharedPtr<T> p) {};

template <typename T>
using Task = std::function<void(SeriesCallback<T>&)>;

template<typename T>
using TaskVector = std::vector<Task<T>>;

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

  auto state = std::make_shared<SeriesState>();

  using Iterator = typename TaskVector<T>::iterator;
  auto iter = std::make_shared<Iterator>(tasks.begin());
  auto is_inside_task = std::make_shared<bool>(false);
  auto callback_called = std::make_shared<bool>(false);
  auto last_error = std::make_shared<ErrorCode>(OK);

  struct SeriesState {
    SeriesCallback<T> callback;
    std::function<void()> invoke_until_async;
    bool is_inside_task;
    bool callback_called;
    ErrorCode last_error = OK;
  };

  // Capture tasks by reference to not copy the vector.  `iter` is bound to the original
  // vector.
  state.callback = [callback_called, results, is_inside_task, iter, last_error, &tasks](ErrorCode error, T result) mutable {
    DebugScope d("callback");
    printf("In callback.  is_inside_task=%d\n", *is_inside_task);
    *callback_called = true;
    results->push_back(result);
    *last_error = error;

    if (!*is_inside_task) {
      while (*iter != tasks.end()) {
        auto task = **iter;
        std::function<void(ErrorCode, int)> cb = [](ErrorCode e, int v) {};
        task(cb);
        ++(*iter);
      }
    }
  };

  // Capture tasks by reference to not copy the vector.  `iter` is bound to the original
  // vector.
  state.invoke_until_async = [callback, callback_called, is_inside_task, iter, &tasks]() mutable {
    DebugScope d("invoke_until_async");
    while (*iter != tasks.end()) {
      *is_inside_task = true;
      *callback_called = false;
      auto task = **iter;
      task(callback);
      *is_inside_task = false;
      ++(*iter);

      if (!*callback_called) {
        break;
      }
    }
  };

  invoke_until_async();
}

}

#endif
