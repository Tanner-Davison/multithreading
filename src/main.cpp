#include "ThreadGaurd.hpp"
#include <exception>
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

int main() {
  std::thread thr(hello);
  std::cout << "\nid: " << thr.get_id() << "\n";
  ThreadGaurd thr_one{std::thread(execfast)};
  ThreadGaurd tgaurd{std::move(thr)};
  return 0;
}
