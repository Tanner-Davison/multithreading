
#include <atomic>
#include <mutex>
#include <print>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std::literals;

namespace shared_mutex_example {
namespace {
int               shared_data = 0;
std::shared_mutex shmut;
std::atomic<int>  reader_count{0};
void              write() {
    std::lock_guard lock(shmut);
    ++shared_data;
}

void read() {
    std::shared_lock lock(shmut);
    std::this_thread::sleep_for(100ms);
    ++reader_count;
}
} // namespace

void run_demo() {
    std::println("=== Shared Mutex Demo ===");
    std::vector<std::thread> threads;

    // Spawn 40 readers
    for (int i = 0; i < 40; ++i) {
        threads.emplace_back(read);
    }

    // Spawn 2 writers
    threads.emplace_back(write);
    threads.emplace_back(write);

    // Join all threads
    for (auto& thr : threads) {
        thr.join();
    }

    std::println("Final shared_data value: {}", shared_data);
    std::println("Readers completed: {}\n", reader_count.load());
}

} // namespace shared_mutex_example
