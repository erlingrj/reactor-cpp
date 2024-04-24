#pragma once
#include "reactor-cpp/time.hh"

namespace reactor {
class Mutex {
public:
  Mutex() {}
  ~Mutex() {}
  void lock() {}
  bool try_lock() {
    return true; // Placeholder return value
  }
  void unlock() {}
};

template <class L> class UniqueLock {
public:
  UniqueLock(L lock) {}
  UniqueLock() {}
  ~UniqueLock() {}
  void lock() {}
  bool try_lock() {
    return true; // Placeholder return value
  }
  void unlock() {}
};

template <class L> class SharedLock {
public:
  SharedLock(L lock) {}
  SharedLock() {}
  ~SharedLock() {}
  void lock() {}
  bool try_lock() {
    return true; // Placeholder return value
  }
  void unlock() {}
};

class SharedMutex {
public:
  SharedMutex() {}

  ~SharedMutex() {}

  void lock() {}

  bool try_lock() {
    return true; // Placeholder return value
  }

  void unlock() {}
};

class CondVar {
public:
  CondVar() {}

  ~CondVar() {}

  void signal() {}
  void broadcast() {}

  void wait(UniqueLock<Mutex>& lock, const std::function<bool(void)>& predicate) {}
  void notify_all() {}
  void notify_one() {}

  bool wait_until(UniqueLock<Mutex>& lock, TimePoint time_point, const std::function<bool(void)>& predicate) {
    return true;
  }
};

class Thread {
public:
  template <class F, class... Args> explicit Thread(F&& f, Args&&... args) {}
  Thread() {}
  ~Thread() {}
  void join() {}
  bool joinable() { return true; }
};
template <class T> class Atomic {
public:
  Atomic() {}
  Atomic(T init) {}
  ~Atomic() {}
  void store(T val, std::memory_order order) {}
  T load(std::memory_order order) const {}
  T load() const {}
  T fetch_add(T val, std::memory_order order) {}
  T fetch_sub(T val, std::memory_order order) {}
  T exchange(T val, std::memory_order order) {}
  operator T() const { return load(); }
};
} // namespace reactor