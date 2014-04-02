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

  // Call begin_ops() when an async operation is started, and then end_op() when it
  // completes (inside the asynchronous callback).  These are also called when a request
  // is first invoked, to ensure the same code path is followed for end_op() in case no
  // async calls were invoked in the request handler.
  void begin_op();
  void end_op();

private:
  Transaction &transaction_;  // Owner.

  int pending_ops_ = 0;

  // Prevent copy.
  AsyncMethods(const AsyncMethods &copy) = delete;

  std::set<SteadyTimerSharedPtr> steady_timers_;
};

#endif // __ASYNC_METHODS_HPP__
