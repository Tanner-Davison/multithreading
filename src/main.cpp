#include "Logger.hpp"
#include "RuntimeSpeed.hpp"
#include "Singleton.hpp"
#include <atomic>
#include <mutex>
#include <print>
#include <random>
#include <shared_mutex>
#include <string_view>
#include <thread>
#include <vector>
using namespace std::literals;

int                       shared_data = 0;
std::shared_mutex         shmut;
std::atomic<int>          reader_count{0};
thread_local std::mt19937 mt;

void write() {
    std::lock_guard lock(shmut);
    ++shared_data;
}

void read() {
    std::shared_lock lock(shmut);
    std::this_thread::sleep_for(100ms);
    ++reader_count;
}
void task() {
    Singleton& single = Singleton::get_instance();
    std::println("{}", static_cast<void*>(&single));
}
void func() {
    std::uniform_real_distribution<double> dist(0, 1);
    for (int i = 0; i < 10; ++i) {
        std::println("{}", dist(mt));
    }
}

int main() {
    RuntimeSpeed&            mainloop = RuntimeSpeed::getProfiler("Main Thread");
    std::vector<std::thread> threads;
    std::this_thread::sleep_for(1000ms);
    {
        auto& threadloops = RuntimeSpeed::getProfiler("threadloops");
        for (int i = 0; i < 40; ++i) {
            threads.emplace_back(read);
        }

        threads.emplace_back(write);
        threads.emplace_back(write);
        for (auto& thr : threads) {
            thr.join();
        }
        threads.clear();

        for (int i = 0; i < 10; ++i) {
            threads.emplace_back(task);
        }
        for (auto& t : threads) {
            t.join();
        }
    }
    threads.clear();
    for (int i = 0; i < 2; ++i) {
        threads.emplace_back(func);
    }
    for (auto& t : threads) {
        t.join();
    }

    std::println("Final shared_data value: {}", shared_data);
    std::println("Readers completed: {}", reader_count.load());
    std::println("access_counter: {}", Singleton::get_access_count());
}
