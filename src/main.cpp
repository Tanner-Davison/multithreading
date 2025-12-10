#include "AtomicCounter.hpp"
#include "MutexLocker.hpp"
#include "MutexTimedTryLock.hpp"
#include "ThreadGuard.hpp"
#include <functional>
#include <iostream>
#include <iterator>
#include <thread>

template <typename ThreadCounters>
void incrementCounters(const std::string& name,
                       ThreadCounters&    counter,
                       int                iterations,
                       int                numThreads) {
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&counter, iterations]() {
            for (int j = 0; j < iterations; ++j) {
                counter.increment();
            }
        });
    };
    for (auto& t : threads) {
        t.join();
    }
    std::cout << name << " : " << counter.get() << std::endl;
}

void TestGaurd() {
    std::mutex                  mtx;
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "---TestGuard Finished Execution---" << std::endl;
}
int main() {
    /* Example of initializing a class with a thread using the move operator */
    std::thread new_thr(TestGaurd);
    ThreadGuard thr(std::move(new_thr));

    const int ITERATIONS{100000};
    const int NUMTHREADS{2};

    AtomicCounter     counter;
    MutexLocker       counter_mtx_lock;
    MutexTimedTryLock counter_timed;

    incrementCounters("Atomic Counter", counter, ITERATIONS, NUMTHREADS);
    incrementCounters("MutexLocker", counter_mtx_lock, ITERATIONS, NUMTHREADS);
    incrementCounters("MutexTryLock", counter_timed, ITERATIONS, NUMTHREADS);

    std::cout << "TryLock Failed Attempts: " << counter_timed.get_attempts()
              << std::endl;

    return 0;
}
