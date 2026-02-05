#include <mutex>
#include <print>
#include <thread>
using namespace std::literals;

namespace livelock_example {
namespace {
std::mutex mut1;
std::mutex mut2;

void funcA() {
    std::this_thread::sleep_for(10ms);
    std::print("After you John!\n");
    std::scoped_lock scoped_lck(mut1, mut2);
    std::this_thread::sleep_for(2s);
    std::print("Thread A has locked both mutexes\n");
}
void funcB() {
    std::this_thread::sleep_for(10ms);
    std::print("After you Cecil!\n");
    std::scoped_lock scoped_lck(mut1, mut2); //  This PREVENTS deadlock!
    std::this_thread::sleep_for(2s);
    std::print("Thread B has locked both mutexes\n");
}

/* TRUE LiveLock Example WARN:Runs Forever*/
void funcC() {
    std::this_thread::sleep_for(10ms);
    bool locked = false;

    while (!locked) {
        std::lock_guard<std::mutex> lck(mut1);
        std::print("After you, Cecil\n");
        std::this_thread::sleep_for(2s);
        locked = mut2.try_lock();
    }
    if (locked)
        std::print("Thread D has locked both mutexes.\n");
}
void funcD() {
    bool locked = false;
    while (!locked) {
        std::lock_guard<std::mutex> lck(mut2);
        std::print("After you, John\n");
        std::this_thread::sleep_for(2s);
        locked = mut1.try_lock();
    }
    if (locked)
        std::print("Thread D has locked both mutexes.\n");
}

} // namespace

void run_livelock_demo() {
    std::thread thrA(funcA);
    std::thread thrB(funcB);

    thrA.join();
    thrB.join();
}
void run_livelock_eternal_conversation() {
    std::thread thrC(funcC);
    std::this_thread::sleep_for(10ms);
    std::thread thrD(funcD);

    thrC.join();
    thrD.join();
}
} // namespace livelock_example
