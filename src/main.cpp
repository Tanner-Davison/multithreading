#include <chrono>
#include <iostream>
#include <mutex>
#include <print>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std::literals;

std::shared_mutex shmut;

std::pair<int, int> p(0, 0);

void write() {
    std::lock_guard<std::shared_mutex> lck_guard(shmut); // exclusive locking
    ++p.first;
}
void read() {
    std::shared_lock lock_guard(shmut); // shared mutex (shmut)
    std::this_thread::sleep_for(1us);
    std::println("Reading: {}", p.first);
    std::cout.flush();
}

int main() {
    std::println("");
    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < 20; ++i) {
        threads.push_back(std::thread(read));
        ++p.second;
    }

    threads.push_back(std::thread(write));
    threads.push_back(std::thread(write));

    for (int i = 0; i < 20; ++i) {
        threads.push_back(std::thread(read));
        ++p.second;
    }

    for (auto& thr : threads) {
        thr.join();
    }

    auto end      = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::println(" Total Execution time: {}ms", duration.count());

    return 0;
}
