#pragma once
#include <mutex>
#include <thread>

using namespace std::literals;
class TryLockFor {
  private:
    int                      count = 0;
    mutable std::timed_mutex mtx_task;
    mutable std::atomic<int> failed_lock_attempts{0};

  public:
    void increment() {
        std::unique_lock<std::timed_mutex> lock(mtx_task, std::defer_lock);
        while (!lock.try_lock_for(1us)) {
            ++failed_lock_attempts;
            std::this_thread::yield();
        }

        count++;
    }
    int get() const {
        std::lock_guard<std::timed_mutex> lock(mtx_task);
        return count;
    }
    int get_failed_lock_count() const {
        std::lock_guard<std::timed_mutex> lock(mtx_task);
        return failed_lock_attempts;
    }

    // std::println( // Use this in main
    //     "{}{:<15}{:>10}{}", GREEN, "  └─ retries", try_lock_for.get_failed_lock_count(), RESET);
};
