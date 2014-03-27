#include "async_methods.hpp"
#include "transaction.hpp"

AsyncMethods::AsyncMethods(Transaction &transaction)
  : transaction_(transaction) {
  DEBUG_CTOR("AsyncMethods");
}

void AsyncMethods::async_wait(TimeoutHandler handler, unsigned int timeout_ms) {
  // http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference/steady_timer.html

  SteadyTimerSharedPtr timer = std::make_shared<boost::asio::steady_timer>(
      transaction_.io_service_,
      std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms));

  // Hold on to timer so it doesn't go out of scope and cancel itself.
  steady_timers_.insert(timer);

  op_started();

  timer->async_wait([=] (const boost::system::error_code& error) {
      std::cout << "handler: " << error.message() << std::endl;
      if (!error) {
        printf("Calling handler\n");
        // Timer expired.
        handler();
      }
      steady_timers_.erase(timer);
      op_ended();
    });
}

void AsyncMethods::op_started() {
  // TODO: Not thread safe.
  pending_ops_++;
}

/*
 * This must be called at completion of each async closure.  It checks if there
 * are any remaining async operations, and if not, notifies the transaction that
 * we are finished.
 */
void AsyncMethods::op_ended() {
  // TODO: Not thread safe.
  pending_ops_--;

  printf("Op ended.  Count: %d\n", pending_ops_);
  if (pending_ops_ == 0) {
    transaction_.notify_no_pending_ops();
  }
}

AsyncMethods::~AsyncMethods() {
  DEBUG_DTOR("AsyncMethods");
}
