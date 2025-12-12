#include "AtomicCounter.hpp"
#include "MutexLocker.hpp"
#include "MutexTimedTryLock.hpp"
#include "MutexUnique.hpp"
#include "ThreadGuard.hpp"
#include "Vector.hpp"
#include <functional>
#include <iostream>
#include <iterator>
#include <print>
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
    std::cout << name << " : " << counter.get() << " Counts" << std::endl;
}

void TestGaurd() {
    std::mutex                  mtx;
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "\n\n---TestGuard Finished Execution---\n" << std::endl;
}

int main() {
    Vector      my_vec;
    std::thread new_thr(TestGaurd);
    ThreadGuard thr(std::move(new_thr));
    int         myint{34};
    const int   ITERATIONS{100000};
    const int   NUMTHREADS{2};

    AtomicCounter     atomic_counter;
    MutexLocker       mutex_counter;
    MutexTimedTryLock timed_mutex_lock;
    MutexUnique       unique_mtx_counter;

    incrementCounters("Atomic Counter", atomic_counter, ITERATIONS, NUMTHREADS);
    incrementCounters("Mutex  Locker", mutex_counter, ITERATIONS, NUMTHREADS);
    incrementCounters(
        "Mutex Timed Lock", timed_mutex_lock, ITERATIONS, NUMTHREADS);
    incrementCounters(
        "Unique Mutex Lock", unique_mtx_counter, ITERATIONS, NUMTHREADS);
    try {
        std::cout << my_vec.pop_back() << std::endl;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    };

    for (int i = 0; i < 20 + 1; ++i) {
        my_vec.push_back(i);
    }
    my_vec.read_vec();

    try {
        std::cout << my_vec.at(39) << std::endl;

    } catch (InvalidIndexException& e) {
        std::cout << "\n---******-----" << e.what()
                  << "\nAttempted index: " << e.index
                  << "\nvector size: " << e.size << "\n---****------"
                  << std::endl;
    }

    return 0;
}
