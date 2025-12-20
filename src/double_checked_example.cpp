
#include "LazyInit.hpp"
#include <mutex>
#include <thread>

namespace double_checked_locking {
namespace {
LazyInit*      ptest = nullptr;
std::once_flag ptest_flag;

void process() {
    std::call_once(ptest_flag, []() { ptest = new LazyInit; });
    ptest->func();
}
} // namespace

void run_demo() {
    std::thread thr1(process);
    std::thread thr2(process);

    thr1.join();
    thr2.join();
}
} // namespace double_checked_locking
