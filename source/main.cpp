#include <chrono>
#include <iostream>
#include <thread>
using namespace std::literals;

void hello() { std::cout << "Started hello Thread!()\n"; }

void wait_sec(int seconds) {

  std::cout << "\n...waiting..\n" << std::flush;
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
  hello();
};
void func(std::thread obj) {
  std::cout << "\nRecieved Thread Object..\n";

  obj.join();
}

int main() {

  std::cout << "Starting thread ...\n";
  std::thread thr(wait_sec, 2);
  std::thread thr4(hello);
  func(std::move(thr));
  func(std::move(thr4));
  return 0;
}
