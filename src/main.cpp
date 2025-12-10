#include "ThreadGaurd.hpp"
#include <atomic>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::literals;

class Counter {
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

class MutexCounter {
  private:
    int        count = 0;
    std::mutex task_mtx;

  public:
    void increment() {
        std::lock_guard<std::mutex> lock(task_mtx);
        ++count;
    }
    int getcount() {
        return count;
    }
};

int main() {
    Counter      counter;
    MutexCounter counter_mtx;

    auto incrementLots = [&counter]() {
        for (int i = 0; i < 100000; ++i) {
            counter.increment();
        };
    };

    auto incrementLots_mtx = [&counter_mtx]() {
        for (int i = 0; i < 100000; ++i) {
            counter_mtx.increment();
        };
    };
    std::thread t1(incrementLots);
    std::thread t2(incrementLots);
    std::thread t3(incrementLots);
    std::thread t4(incrementLots_mtx);
    std::thread t5(incrementLots_mtx);
    std::thread t6(incrementLots_mtx);

    t1.join();
    t2.join();
    t3.join();
    std::cout << "\n" << counter.get() << std::endl;
    t4.join();
    t5.join();
    t6.join();

    std::cout << "\n" << counter_mtx.getcount() << std::endl;
    return 0;
}
