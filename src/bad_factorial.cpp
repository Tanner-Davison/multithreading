#include <mutex>
#include <print>
#include <thread>
#include <vector>

namespace recursive_mutex_example {

namespace {
std::recursive_mutex rmut;

int bad_factorial(int n) {
    if (n <= 1) {
        std::println("Returning {}", 1);
        return 1;
    }
    std::lock_guard<std::recursive_mutex> lck(rmut);
    int                                   retval = n * bad_factorial(n - 1);
    std::println("Returning {}", retval);
    return retval;
}
} // namespace

void run_bad_factorial() {
    std::println("\n=== Recursive Mutex Demo ===");
    std::println("Computing 5!\n recursively with recursive_mutex:");
    int result = bad_factorial(5);
    std::println("Final result: {}", result);

    std::println("\nTesting with multiple threads:");
    std::vector<std::thread> threads;
    for (int i = 3; i <= 5; ++i) {
        threads.emplace_back([i]() {
            std::println("Thread computing {}!", i);
            bad_factorial(i);
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}

} // namespace recursive_mutex_example
