#include "RuntimeSpeed.hpp"
#include <atomic>
#include <chrono>
#include <mutex>
#include <print>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std::literals;

std::shared_mutex shmut;
std::atomic<int>  reader_count{0};

int shared_data = 0;

void write() {
    std::lock_guard lock(shmut);
    ++shared_data;
}

void read() {
    std::shared_lock lock(shmut);
    std::this_thread::sleep_for(100ms);
    ++reader_count;
}

int main() {
    RuntimeSpeed             runtime("Main Thread");
    std::vector<std::thread> threads;

    std::println("");
    for (int i = 0; i < 40; ++i) {
        threads.emplace_back(read);
    }

    std::println("elapsed_time: {}ms", runtime.elapsed_time());
    threads.emplace_back(write);
    threads.emplace_back(write);
    for (auto& thr : threads) {
        thr.join();
    }

    std::println("Final value: {}", shared_data);
    std::println("Readers completed: {}", reader_count.load());
}
