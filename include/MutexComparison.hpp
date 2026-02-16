#pragma once
#include <condition_variable>
#include <mutex>

/*
 * MutexLocker vs MutexUnique — When to use which?
 *
 * std::lock_guard  → Simple RAII lock. Locks on construction, unlocks on destruction.
 *                    Use when you just need to protect a critical section. Lightweight.
 *
 * std::unique_lock → Flexible RAII lock. Supports:
 *                    - Deferred locking (std::defer_lock)
 *                    - Manual lock/unlock mid-scope
 *                    - try_lock / try_lock_for / try_lock_until
 *                    - Condition variable integration (cv.wait REQUIRES unique_lock)
 *                    - Moveable (can transfer lock ownership)
 *                    Use when you need any of the above. Slightly heavier.
 *
 * Rule of thumb: Start with lock_guard. Switch to unique_lock only when you need
 * its extra capabilities.
 */

// Simple case: lock_guard is all you need
class MutexLocker {
  private:
    mutable int        count = 0;
    mutable std::mutex task_mtx;

  public:
    void increment() {
        std::lock_guard<std::mutex> lock(task_mtx);
        ++count;
    }
    int get() const {
        std::lock_guard<std::mutex> lock(task_mtx);
        return count;
    }
};

// unique_lock shines when you need flexibility
class MutexUnique {
    mutable int                 count{0};
    mutable std::mutex          mtx;
    mutable std::condition_variable cv;

  public:
    // Demonstrates manual unlock before notify (avoids waking thread into contention)
    void increment() {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        lock.unlock(); // <-- can't do this with lock_guard
        cv.notify_one();
    }

    // Demonstrates condition variable wait (REQUIRES unique_lock, lock_guard won't compile)
    int wait_until_at_least(int target) const {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return count >= target; });
        return count;
    }

    int get() const {
        std::unique_lock<std::mutex> lock(mtx);
        return count;
    }
};
