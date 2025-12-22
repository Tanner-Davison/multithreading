#include <mutex>
#include <print>
#include <thread>

using namespace std::literals;
namespace pre_seventeen_deadlock_example {
namespace {
std::mutex mtxA, mtxB;

void taskA() {
    std::lock(mtxA, mtxB);
    std::println("Thread A Trying to adopt mutex 1 and 2");
    std::unique_lock<std::mutex> lock(mtxA, std::adopt_lock);
    std::unique_lock<std::mutex> lock2(mtxB, std::adopt_lock);
    std::println("Thread A Took ownership of lock 1 and 2\nA is Unlocking ");
}
void taskB() {
    std::lock(mtxA, mtxB);
    std::println("Thread B Trying to adopt mutex 1 and 2");
    std::unique_lock<std::mutex> lock(mtxA, std::adopt_lock);
    std::unique_lock<std::mutex> lock2(mtxB, std::adopt_lock);
    std::println("Thread B Took ownership of lock 1 and 2\nB is unlocking.");
}
void deferTaskA() {
    std::println("doing some prep work for 50ms...");

    std::this_thread::sleep_for(50ms);
    std::unique_lock<std::mutex> lock(mtxA, std::defer_lock);
    std::unique_lock<std::mutex> lock2(mtxB, std::defer_lock);
    std::println("A is now attempting deffered locking");
    std::lock(lock, lock2); // passing the unique lock (lock)
    std::println("Thread A Took ownership of lock 1 and 2\nA is Unlocking ");
}
void deferTaskB() {
    std::println("doing some prep work for 50ms...");
    std::this_thread::sleep_for(50ms);
    std::unique_lock<std::mutex> lock(mtxA, std::defer_lock);
    std::unique_lock<std::mutex> lock2(mtxB, std::defer_lock);
    std::println("B is now attempting deffered locking");
    std::lock(lock, lock2); // passing the unique lock (lock)
    std::println("Thread B Took ownership of lock 1 and 2\nB is Unlocking ");
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
} // namespace pre_seventeen_deadlock_example
