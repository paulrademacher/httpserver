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
using SeriesCallback = std::function<void(ErrorCode error, T result)>;

template<typename T>
void noop_series_callback(ErrorCode e, T result) {};

template<typename T>
using SeriesCompletionCallback = std::function<void(ErrorCode, std::vector<T>)>;

template<typename T>
void noop_series_final_callback(ErrorCode e, std::vector<T> p) {};

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
    const std::function<void(ErrorCode, std::vector<T>&)> &final_callback=noop_series_final_callback<T>) {

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
    const std::function<void(ErrorCode, std::vector<T>&)> &final_callback=noop_series_final_callback<T>) {

  struct SeriesState {
    SeriesCallback<T> callback;
    std::function<void()> invoke_until_async;
    typename TaskVector<T>::iterator iter;
    bool is_inside_task;
    bool callback_called;
    bool is_finished = false;
    ErrorCode last_error = OK;
    std::vector<T> results;
    std::vector<Task<T>> *tasks;

    SeriesState() { printf("> SeriesState\n"); }
    ~SeriesState() { printf("< SeriesState\n"); }

    //    SeriesState(std::vector<Task<T>> *tasks_in) : tasks(tasks_in) {};
  };

  auto state = std::make_shared<SeriesState>();
  state->tasks = &tasks;
  state->iter = tasks.begin();

  // If task list is empty, invoke the final callback immediately with a success code and
  // an empty results list.
  if (state->tasks->size() == 0) {
    final_callback(OK, state->results);
    return;
  }

  // Capture tasks by reference to not copy the vector.  `iter` is bound to the original
  // vector.
  state->callback = [state](ErrorCode error, T result) mutable {
    if (state->is_finished) {
      printf("ERROR ********************\n");  // REMOVE ME
    }

    DebugScope d("callback");
    printf("In callback.  is_inside_task=%d\n", state->is_inside_task);
    state->callback_called = true;
    state->results.push_back(result);
    state->last_error = error;

    if (!state->is_inside_task) {
      state->invoke_until_async();

      if (state->is_finished) {
        printf("cb state count: %d\n", state.use_count());
        state.reset();
      }
    }
  };

  // Capture tasks by reference to not copy the vector.  `iter` is bound to the original
  // vector.
  state->invoke_until_async = [state]() mutable {
    if (state->is_finished) {
      printf("ERROR ********************\n");  // REMOVE ME
    }

    DebugScope d("invoke_until_async");
    while (state->iter != state->tasks->end()) {
      state->is_inside_task = true;
      state->callback_called = false;
      auto task = *(state->iter);
      task(state->callback);
      state->is_inside_task = false;
      ++(state->iter);

      if (!state->callback_called) {
        break;
      }
    }

    if (state->iter == state->tasks->end() && state->callback_called) {
      state->is_finished = true;

      // We're done.
      printf("invoke state count: %d\n", state.use_count());
      state.reset();
    }
  };

  state->invoke_until_async();

  printf("series state count: %d\n", state.use_count());
  state.reset();
}

}

#endif
