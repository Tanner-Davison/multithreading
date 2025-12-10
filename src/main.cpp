#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class MutexCounter {
  private:
    int        count = 0;
    std::mutex mtx;

  public:
    void increment() {
        std::lock_guard<std::mutex> lock(mtx);
        ++count;
    }
    int get() const {
        return count;
    }
};

class AtomicCounter {
  private:
    std::atomic<int> count{0};

  public:
    void increment() {
        ++count;
    }
    int get() const {
        return count.load();
    }
};

class SingleThreadCounter {
  private:
    int count = 0;

  public:
    void increment() {
        ++count;
    }
    int get() const {
        return count;
    }
};

template <typename Counter>
void runBenchmark(const std::string& name, int numThreads, int iterations) {
    Counter counter;

    auto start = std::chrono::high_resolution_clock::now();

    if (numThreads == 1) {
        // Single-threaded
        for (int i = 0; i < iterations; ++i) {
            counter.increment();
        }
    } else {
        // Multi-threaded
        std::vector<std::thread> threads;
        int                      perThread = iterations / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&counter, perThread]() {
                for (int j = 0; j < perThread; ++j) {
                    counter.increment();
                }
            });
        }

        for (auto& t : threads) {
            t.join();
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << name << " (" << numThreads << " thread"
              << (numThreads > 1 ? "s" : "") << "): " << duration.count()
              << "ms (result: " << counter.get() << ")\n";
}

int main() {
    const int ITERATIONS = 1000000;

    std::cout << "=== Benchmark: " << ITERATIONS << " increments ===\n\n";

    // Single-threaded baseline
    std::cout << "Single-threaded baseline:\n";
    runBenchmark<SingleThreadCounter>("Plain int", 1, ITERATIONS);
    std::cout << "\n";

    // Atomic - single thread
    std::cout << "Atomic (1 thread):\n";
    runBenchmark<AtomicCounter>("std::atomic", 1, ITERATIONS);
    std::cout << "\n";

    // Atomic - multi-threaded
    std::cout << "Atomic (multi-threaded):\n";
    runBenchmark<AtomicCounter>("std::atomic", 2, ITERATIONS);
    runBenchmark<AtomicCounter>("std::atomic", 4, ITERATIONS);
    std::cout << "\n";

    // Mutex - single thread
    std::cout << "Mutex (1 thread):\n";
    runBenchmark<MutexCounter>("std::mutex", 1, ITERATIONS);
    std::cout << "\n";

    // Mutex - multi-threaded (THIS IS YOUR CODE)
    std::cout << "Mutex (multi-threaded) - YOUR APPROACH:\n";
    runBenchmark<MutexCounter>("std::mutex", 2, ITERATIONS);
    runBenchmark<MutexCounter>("std::mutex", 4, ITERATIONS);
    std::cout << "\n";

    std::cout << "=== Analysis ===\n";
    std::cout << "- Single-threaded: Fastest for this workload\n";
    std::cout << "- Atomic: Slower than plain int, but thread-safe\n";
    std::cout << "- Mutex (multi-thread): SLOWEST due to lock contention\n";
    std::cout
        << "\nConclusion: For tiny critical sections with high contention,\n";
    std::cout << "            single-threaded or atomic is faster!\n";

    return 0;
}
/* #include "ThreadGaurd.hpp"
#include <atomic>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std::literals;

class Counter {
  private:
    std::atomic<int> count{0};

  public:
    void increment() {
        ++count;
    }
    int get() const {
        return count.load();
    }
};

class MutexCounter {
  private:
    int        count = 0;
    std::mutex task_mtx;

  public:
    void increment() {
        std::lock_guard<std::mutex> lock(task_mtx);
        ++count;
    }
    int getcount() {
        return count;
    }
};

int main() {
    Counter      counter;
    MutexCounter counter_mtx;

    auto incrementLots = [&counter]() {
        for (int i = 0; i < 100000; ++i) {
            counter.increment();
        };
    };

    auto incrementLots_mtx = [&counter_mtx]() {
        for (int i = 0; i < 100000; ++i) {
            counter_mtx.increment();
        };
    };
    std::thread t1(incrementLots);
    std::thread t2(incrementLots);
    std::thread t3(incrementLots);
    std::thread t4(incrementLots_mtx);
    std::thread t5(incrementLots_mtx);
    std::thread t6(incrementLots_mtx);

    t1.join();
    t2.join();
    t3.join();
    std::cout << "\n" << counter.get() << std::endl;
    t4.join();
    t5.join();
    t6.join();

    std::cout << "\n" << counter_mtx.getcount() << std::endl;
    return 0;
} */
