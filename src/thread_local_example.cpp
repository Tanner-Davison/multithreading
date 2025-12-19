#include <print>
#include <random>
#include <thread>

namespace thread_local_example {
namespace {
thread_local std::mt19937 mt;

int RADNOM_NUMS{3};
int THREADS{2};
} // namespace

void gen_randoms() {
    std::uniform_real_distribution<double> dist(0, 1);
    for (int i = 0; i < RADNOM_NUMS; ++i) {
        std::println("{}", dist(mt));
    }
}
void run_demo() {
    std::println("\n=== Thread-Local RNG Demo ===");
    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(gen_randoms);
    }
    for (auto& t : threads) {
        t.join();
    }
    std::println("\n");
}
} // namespace thread_local_example
