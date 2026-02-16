#pragma once
#include <chrono>
#include <mutex>
#include <thread>

using namespace std::literals;

/*
 * TryLockFor vs TryLockUntil — Two flavors of timed locking
 *
 * Both require std::timed_mutex (not regular std::mutex).
 *
 * try_lock_for(duration)     → "Try for up to N microseconds from NOW"
 *                               Relative timeout. Simpler for retry loops.
 *
 * try_lock_until(time_point) → "Try until this wall-clock deadline"
 *                               Absolute timeout. Better when you have a
 *                               hard deadline that shouldn't drift with retries.
 *
 * In practice: try_lock_for is more common. try_lock_until is useful when
 * you're coordinating with external deadlines (e.g., "must finish by T+500ms").
 */

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
};

class TryLockUntil {
  private:
    int                      count{0};
    mutable std::timed_mutex mtx_task;
    std::atomic<int>         failed_lock_count{0};

  public:
    void increment() {
        std::unique_lock<std::timed_mutex> lock(mtx_task, std::defer_lock);
        auto deadline = std::chrono::system_clock::now() + 1us;
        while (!lock.try_lock_until(deadline)) {
            std::this_thread::yield();
            deadline = std::chrono::system_clock::now() + 1us; // reset deadline each retry
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
};
