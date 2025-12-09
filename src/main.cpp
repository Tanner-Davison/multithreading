#include "ThreadGaurd.hpp"
#include <atomic>
#include <exception>
#include <functional>
#include <iostream>
#include <thread>
using namespace std::literals;

void hello() {
  std::cout << "\nid: " << std::this_thread::get_id() << "\n";
  try {
    throw std::exception();
  } catch (std::exception &e) {
    std::cout << "Exception Caught! " << e.what() << "\n";
  }
  std::cout << "Hello World!\n";
}

void execfast() { std::cout << "\nExecuting fast!" << std::endl; };

std::thread func() {
  std::thread thr(hello);
  return thr;
}

void writeAlot(std::atomic<int> &num) {
  for (int i = 0; i < 100000; ++i) {
    ++num;
  }
};

int main() {
  // int count = 0;

  // std::thread thr1(writeAlot, std::ref(count));
  // std::thread thr2(writeAlot, std::ref(count));
  // std::thread thr3(writeAlot, std::ref(count));

  // thr1.join();
  // thr2.join();
  // thr3.join();
  // std::cout << "\n" << count << "\n";

  std::cout << "\n";
  for (int run = 0; run < 10; ++run) {
    std::atomic<int> count = 0;
    std::thread thr1(writeAlot, std::ref(count));
    std::thread thr2(writeAlot, std::ref(count));
    std::thread thr3(writeAlot, std::ref(count));

    thr1.join();
    thr2.join();
    thr3.join();
    std::cout << "Run " << run + 1 << ": " << count << std::endl;
  }
  return 0;
}
