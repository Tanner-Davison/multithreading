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
  void increment() { ++count; }
  int get() const { return count.load(); }
};

int main() {
  Counter counter;

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

  std::cout << "\n" << counter.get() << std::endl;
  return 0;
}
