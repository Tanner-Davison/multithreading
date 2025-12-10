#include "ThreadGaurd.hpp"
#include <atomic>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::literals;

class AtomicCounter {
  private:
    std::atomic<int> count{0};

  public:
    void increment() {
        ++count;
    }
    int get() const {
        return count.load();
    }
};

class MutexLocker {
  private:
    int        count = 0;
    std::mutex task_mtx;

  public:
    void increment() {
        std::lock_guard<std::mutex> lock(task_mtx);
        ++count;
    }
    int get() {
        return count;
    }
};
class MutexTryLock {
  private:
    int              count = 0;
    std::mutex       mtx_task;
    std::atomic<int> attempt_unlock_count{0};

  public:
    void increment() {
        while (!mtx_task.try_lock()) {
            attempt_unlock_count++;
        }
        count++;
        mtx_task.unlock();
    }
    int get() {
        std::lock_guard<std::mutex> lock(mtx_task);
        return count;
    }
    int get_attempts() {
        std::lock_guard<std::mutex> lock(mtx_task);
        return attempt_unlock_count;
    }
};

int main() {
    AtomicCounter counter;
    MutexLocker   counter_mtx;
    MutexTryLock  counter_try;

    auto AtomicIncrement = [&counter]() {
        for (int i = 0; i < 100000; ++i) {
            counter.increment();
        };
    };

    auto MutextLockerIncrement = [&counter_mtx]() {
        for (int i = 0; i < 100000; ++i) {
            counter_mtx.increment();
        };
    };

    auto MutexTryLockIncrement = [&counter_try]() {
        for (int i = 0; i < 100000; ++i) {
            counter_try.increment();
        };
    };
    std::thread t1(AtomicIncrement);
    std::thread t2(AtomicIncrement);
    std::thread t3(MutextLockerIncrement);
    std::thread t4(MutextLockerIncrement);
    std::thread t5(MutexTryLockIncrement);
    std::thread t6(MutexTryLockIncrement);

    t1.join();
    t2.join();
    std::cout << "\n" << counter.get() << std::endl;
    t3.join();
    t4.join();
    std::cout << "\n" << counter_mtx.get() << std::endl;
    t5.join();
    t6.join();
    std::cout << "\n" << counter_try.get() << std::endl;
    std::cout << "\n" << counter_try.get_attempts() << std::endl;
    return 0;
}
