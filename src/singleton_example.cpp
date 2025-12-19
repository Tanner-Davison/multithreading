#include "Singleton.hpp"
#include <print>
#include <thread>
#include <vector>

namespace singleton_example {
namespace {
int THREADS{5};
}
void task() {
    Singleton& single = Singleton::get_instance();
    std::println("{}", static_cast<void*>(&single));
}
void run_demo() {
    std::println("=== Singleton Example ===");
    std::vector<std::thread> threads;

    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(task);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::println("Singleton Access Count: {}\n", Singleton::get_access_count());
}
} // namespace singleton_example
