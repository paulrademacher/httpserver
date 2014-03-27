#ifndef __ASYNC_METHODS_HPP__
#define __ASYNC_METHODS_HPP__

#include <set>

#include "common.hpp"

class Transaction;

// AsyncMethods utility methods.
class AsyncMethods {
public:
  AsyncMethods(Transaction &transaction);
  ~AsyncMethods();

  void async_wait(TimeoutHandler handler, unsigned int timeout_ms);

private:
  Transaction &transaction_;  // Owner.

  int pending_ops_ = 0;

  void op_started();
  void op_ended();

  std::set<SteadyTimerSharedPtr> steady_timers_;
};

#endif // __ASYNC_METHODS_HPP__
