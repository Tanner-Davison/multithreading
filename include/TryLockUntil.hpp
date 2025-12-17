
#pragma once
#include <chrono>
#include <cstdlib>
#include <mutex>
#include <print>
#include <thread>

using namespace std::literals;
class TryLockUntil {
  private:
    int                      count{0};
    mutable std::timed_mutex mtx_task;
    std::atomic<int>         failed_lock_count{0};

  public:
    void increment() {
        std::unique_lock<std::timed_mutex> lock(mtx_task, std::defer_lock);
        auto                               deadline = std::chrono::system_clock::now() + 1us;
        while (!lock.try_lock_until(deadline)) {
            std::this_thread::yield();
            deadline = std::chrono::system_clock::now() + 1us;
            ++failed_lock_count;
        }
        count++;
    }
    int get() const {
        std::lock_guard<std::timed_mutex> lock(mtx_task);
        return count;
    }
    int get_failed_lock_count() const {
        std::lock_guard<std::timed_mutex> lock(mtx_task);
        return failed_lock_count;
    }

    // Use this in main
    // std::println(
    //     "{}{:<15}{:>10}{}", GREEN, "  └─ retries", try_lock_until.get_failed_lock_count(),
};
