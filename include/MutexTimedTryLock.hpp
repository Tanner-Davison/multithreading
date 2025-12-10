#pragma once
#include <mutex>
#include <thread>

using namespace std::literals;
class MutexTimedTryLock {
  private:
    int                      count = 0;
    mutable std::timed_mutex mtx_task;
    mutable std::atomic<int> attempt_unlock_count{0};

  public:
    void increment() {
        std::unique_lock<std::timed_mutex> lock(mtx_task, std::defer_lock);
        while (!lock.try_lock_for(5ms)) {
            ++attempt_unlock_count;
            std::this_thread::yield();
        }
        count++;
        mtx_task.unlock();
    }
    int get() const {
        std::lock_guard<std::timed_mutex> lock(mtx_task);
        return count;
    }
    int get_attempts() const {
        std::lock_guard<std::timed_mutex> lock(mtx_task);
        return attempt_unlock_count;
    }
};
