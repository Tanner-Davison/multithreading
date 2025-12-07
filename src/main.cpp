#include <exception>
#include <iostream>
#include <thread>
using namespace std::literals;

void hello() {
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

  std::thread thr{func()};
  std::thread thr1{execfast};
  thr1.join();
  thr.join();
  return 0;
}
