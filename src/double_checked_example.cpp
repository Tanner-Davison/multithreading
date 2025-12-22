
#include "LazyInit.hpp"
#include <atomic>
#include <mutex>
#include <thread>

namespace double_checked_locking {
namespace {
std::unique_ptr<LazyInit> ptest;
std::once_flag            ptest_flag;

// for double_locking example
std::atomic<LazyInit*> ptest1{nullptr};
std::mutex             mut;
//
void double_locking() {
    LazyInit* tmp = ptest1.load(std::memory_order_acquire);
    if (tmp == nullptr) {
        std::lock_guard<std::mutex> lock(mut);
        tmp = ptest1.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new LazyInit();
            ptest1.store(tmp, std::memory_order_release);
        }
    }

    tmp->func();
}

void call_once() {
    std::call_once(ptest_flag, []() { ptest = std::make_unique<LazyInit>(); });
    ptest->func();
}
} // namespace

void run_safer_call_once() {
    std::thread thr1(call_once);
    std::thread thr2(call_once);

    thr1.join();
    thr2.join();
}

void run_double_check_demo() {
    std::thread thr1(double_locking);
    std::thread thr2(double_locking);

    thr1.join();
    thr2.join();
}
} // namespace double_checked_locking
