#include "ThreadGaurd.hpp"
#include <atomic>
#include <exception>
#include <functional>
#include <iostream>
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
        task_mtx.lock();
        ++count;
        task_mtx.unlock();
    }
    int getcount() {
        return count;
    }
};

int main() {
    MutexCounter counter;

    auto incrementLots = [&counter]() {
        for (int i = 0; i < 100000; ++i) {
            counter.increment();
        };
    };

    std::thread t1(incrementLots);
    std::thread t2(incrementLots);
    std::thread t3(incrementLots);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "\n" << counter.getcount() << std::endl;
    return 0;
}
