#pragma once

#ifndef __ASYNC_ASYNC_HPP__
#define __ASYNC_ASYNC_HPP__

#include <boost/asio.hpp>

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


void *global_callback;

template<typename T>
void series(boost::asio::io_service *io_service,
    std::vector<Task<T>> tasks,
    std::function<void(ErrorCode, VectorSharedPtr<T>)> final_callback=nullptr) {

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
  SeriesCallback<T> callback = [&tasks, &task_iter, final_callback, results, &callback](ErrorCode error, T result) mutable {
    results->push_back(result);
    if (error == OK) {
      ++task_iter;
      if (task_iter == tasks.end()) {
        if (final_callback) {
          final_callback(error, results);
        }
      } else {
        //        (*task_iter)(callback);
      }
    } else {
      if (final_callback) {
        final_callback(error, results);
      }
    }
  };

  //  (*task_iter)(callback);
}

}

#endif
