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
using Task = std::function<void(SeriesCallback<T>)>;

template<typename T>
using TaskVector = std::vector<Task<T>>;

template<typename T>
void noop_series_final_callback(ErrorCode e, VectorSharedPtr<T> p) {};

// `tasks` and `final_callback` are passed by reference.  It is the responsibility of the
// caller to ensure that their lifetime exceeds the lifetime of the series call.
template<typename T>
void series(std::vector<Task<T>> &tasks,
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
    printf("err=%d   iter: %x %x\n", error, task_iter, tasks.end());
    results->push_back(result);
    if (error == OK) {
      ++task_iter;
      printf("err=%d       : %x %x\n", error, task_iter, tasks.end());
      if (task_iter == tasks.end()) {
        printf("LAST\n");
        final_callback(error, results);
      } else {
        printf("2\n");
        (*task_iter)(callback);
      }
    } else {
      printf("3\n");
      final_callback(error, results);
    }
  };

  (*task_iter)(callback);
}

}

#endif
