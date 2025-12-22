#include <mutex>
#include <print>
#include <thread>
/* THESE CAN ALL BE AVOIDED WITH std::scoped_lock<std::mutex> scopedLock(mtx1, mtx2)*/
using namespace std::literals;
namespace pre_seventeen_deadlock_example {
namespace {
std::mutex mtx1, mtx2;

void taskA() {
    std::lock(mtx1, mtx2);
    std::println("Thread A Trying to adopt mutex 1 and 2");
    std::unique_lock<std::mutex> lock(mtx1, std::adopt_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::adopt_lock);
    std::println("Thread A Took ownership of lock 1 and 2\nA is Unlocking ");
}
void taskB() {
    std::lock(mtx1, mtx2);
    std::println("Thread B Trying to adopt mutex 1 and 2");
    std::unique_lock<std::mutex> lock(mtx1, std::adopt_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::adopt_lock);
    std::println("Thread B Took ownership of lock 1 and 2\nB is unlocking.");
}
void deferTaskA() {
    std::println("doing some prep work for 50ms...");

    std::this_thread::sleep_for(50ms);
    std::unique_lock<std::mutex> lock(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
    std::println("A is now attempting deffered locking");
    std::lock(lock, lock2); // passing the unique lock (lock)
    std::println("Thread A Took ownership of lock 1 and 2\nA is Unlocking ");
}
void deferTaskB() {
    std::println("doing some prep work for 50ms...");
    std::this_thread::sleep_for(50ms);
    std::unique_lock<std::mutex> lock(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> lock2(mtx2, std::defer_lock);
    std::println("B is now attempting deffered locking");
    std::lock(lock, lock2); // passing the unique lock (lock)
    std::println("Thread B Took ownership of lock 1 and 2\nB is Unlocking ");
}
void tryLockTaskA() {
    std::unique_lock<std::mutex> uniq_lk1(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> uniq_lk2(mtx2, std::defer_lock);
    std::println("Thread A Trying to lock mutex 1 & 2");

    auto idx = std::try_lock(uniq_lk1, uniq_lk2);
    if (idx != -1) {
        std::println(" try_lock failed to lock mutex 1 & 2");
    } else {
        std::println("Thread A has locked mutexes 1 & 2\n sleeping for 50ms");
        std::this_thread::sleep_for(50ms);
        std::println("Thread A releasing Locks.");
    }
}

void tryLockTaskB() {
    std::unique_lock<std::mutex> uniq_lk1(mtx1, std::defer_lock);
    std::unique_lock<std::mutex> uniq_lk2(mtx2, std::defer_lock);
    std::println("Thread B Trying to lock mutex 1 & 2");

    auto idx = std::try_lock(uniq_lk1, uniq_lk2);
    if (idx != -1) {
        std::println(" try_lock failed to lock mutex 1 & 2 on mutex: {}", idx);
    } else {
        std::println("Thread B has locked mutexes 1 & 2\n sleeping for 50ms");
        std::this_thread::sleep_for(50ms);
        std::println("Thread B releasing Locks.");
    }
}
} // namespace

void run_demo() {
    std::thread thrA(taskA);
    std::thread thrB(taskB);
    thrA.join();
    thrB.join();
}

void run_demo_using_defer() {
    std::thread thrA(deferTaskA);
    std::thread thrB(deferTaskB);

    thrA.join();
    thrB.join();
}
void run_try_lock_demo() {
    std::thread thrA(tryLockTaskA);
    std::thread thrB(tryLockTaskB);

    thrA.join();
    thrB.join();
}
} // namespace pre_seventeen_deadlock_example
