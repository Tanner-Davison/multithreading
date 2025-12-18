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
    auto                     start = std::chrono::steady_clock::now();
    std::vector<std::thread> threads;

    std::println("");
    for (int i = 0; i < 40; ++i) {
        threads.emplace_back(read);
    }

    threads.emplace_back(write);
    threads.emplace_back(write);

    for (auto& thr : threads) {
        thr.join();
    }

    auto end      = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::println("Final value: {}", shared_data);
    std::println("Readers completed: {}", reader_count.load());
    std::println("Total Execution time: {}ms", duration.count());
}
