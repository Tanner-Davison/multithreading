#include <condition_variable>
#include <iostream>
#include <mutex>
#include <print>
#include <thread>
#include <vector>

using namespace std::literals;
namespace condition_variable_example {
namespace {
std::mutex              mtx;
std::condition_variable cv;
std::string             sdata;
bool                    ready = false;

void reader(int id) {
    int                          line = id + 1;
    std::unique_lock<std::mutex> uniq_lock(mtx);
    std::print("\033[{};1H\033[KReader {} waiting...", line, id);
    std::cout << std::flush;
    uniq_lock.unlock();

    std::this_thread::sleep_for(500ms);

    uniq_lock.lock();
    cv.wait(uniq_lock, [&] { return ready; });
    std::print("\033[{};1H\033[KReader {} got: {}", line, id, sdata);
    std::cout << std::flush;
}

void writer(int id) {
    int line = id + 1;
    std::this_thread::sleep_for(2s); // let readers get into wait state first
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::print("\033[{};1H\033[KWriter {} modifying data...", line, id);
        std::cout << std::flush;
        std::this_thread::sleep_for(5s);
        sdata = "Populated.";
        ready = true;
    }
    std::print("\033[{};1H\033[KWriter {} notifying all readers", line, id);
    std::cout << std::flush;
    cv.notify_all();
}

} // namespace
void run_demo() {
    std::print("\033[2J\033[1;1H");
    std::cout << std::flush;
    std::vector<std::thread> threads;
    // 10 readers (id 0-9), 5 writers (id 10-14)
    for (int i = 0; i < 15; i++) {
        if (i < 10) {
            threads.emplace_back(reader, i);
        } else {
            threads.emplace_back(writer, i);
        }
    }
    for (auto& t : threads) {
        t.join();
    }
    // move cursor below all output
    std::print("\033[17;1H\n");
    std::cout << std::flush;
}
} // namespace condition_variable_example
