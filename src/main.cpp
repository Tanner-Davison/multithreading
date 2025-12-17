#include "MutexUnique.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <print>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std::literals;

std::mutex        mut;
std::shared_mutex shmut;

int                         x{0};
std::pair<std::string, int> p("Initial", 0);

void write() {
    std::lock_guard<std::shared_mutex> lck_guard(shmut); // exclusive locking
    p.first = "Modified";
    ++p.second;
}
void read() {
    // std::shared_lock lock_guard(shmut); // shared locking example
    std::string temp = p.first;
    std::this_thread::sleep_for(1us);
    std::println("{} : {} ", temp, p.second);
}

int main() {
    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < 20; ++i) {
        threads.push_back(std::thread(read));
    }

    threads.push_back(std::thread(write));
    threads.push_back(std::thread(write));

    for (int i = 0; i < 20; ++i) {
        threads.push_back(std::thread(read));
    }
    for (auto& thr : threads) {
        thr.join();
    }

    auto end      = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Total execution time: " << duration.count() << "ms\n";

    return 0;
}
