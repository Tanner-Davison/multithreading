#pragma once
#include <mutex>
class MutexLocker {
  private:
    mutable int        count = 0;
    mutable std::mutex task_mtx;

  public:
    void increment() {
        std::lock_guard<std::mutex> lock(task_mtx);
        ++count;
    }
    int get() {
        return count;
    }
};
