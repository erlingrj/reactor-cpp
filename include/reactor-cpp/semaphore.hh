/*
 * Copyright (C) 2021 TU Dresden
 * All rights reserved.
 *
 * Authors:
 *   Christian Menard
 */

#ifndef REACTOR_CPP_SEMAPHORE_HH
#define REACTOR_CPP_SEMAPHORE_HH

#include "reactor-cpp/platform/platform.hh"
#include <atomic>
#include <condition_variable>

namespace reactor {

class Semaphore {
private:
  int count_;
  Mutex mutex_{};
  CondVar cv_{};

public:
  explicit Semaphore(int count)
      : count_(count) {}

  void release(int increment) {
    {
      std::lock_guard<Mutex> lock_guard(mutex_);
      count_ += increment;
    }
    cv_.notify_all();
  }

  void acquire() {
    UniqueLock<Mutex> lock_guard(mutex_);
    cv_.wait(lock_guard, [&]() { return count_ != 0; });
    count_--;
  }
};

} // namespace reactor

#endif // REACTOR_CPP_SEMAPHORE_HH
