#include "AtomicCounter.hpp"
#include "MutexLocker.hpp"
#include "MutexUnique.hpp"
#include "ThreadGuard.hpp"
#include "TryLockFor.hpp"
#include "TryLockUntil.hpp"
#include "Vector.hpp"
#include <iostream>
#include <print>
#include <thread>
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

std::recursive_mutex rmut; // used in bad_factorial;

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
    std::println("{:<15} {:>10}", name, counter.get());
}
void TestGaurd() {
    std::mutex                  mtx;
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "\n\n---TestGuard Finished "
                 "Execution---\n"
              << std::endl;
}

int bad_factorial(int n) {
    if (n <= 1) {
        std::println("Returning {}", 1);
        return 1;
    }
    std::lock_guard<std::recursive_mutex> lck(rmut);
    // Start of Critical Section
    int retval = n * bad_factorial(n - 1);
    std::println("Returning {}", retval);
    // End of Critical Section
    return retval;
}

int main() {
    Vector      my_vec;
    std::thread new_thr(TestGaurd);
    ThreadGuard thr(std::move(new_thr));

    int       myint{34};
    const int ITERATIONS{100000};
    const int NUMTHREADS{2};

    AtomicCounter atomic_lck;
    MutexLocker   lck_guard;
    TryLockFor    try_lock_for;
    MutexUnique   unique_lck;
    TryLockUntil  try_lock_until;

    std::println("{:<15} {:>10}", "Name", "Count");
    incrementCounters("Atomic_counter", atomic_lck, ITERATIONS, NUMTHREADS);
    incrementCounters("Lock_guard", lck_guard, ITERATIONS, NUMTHREADS);
    incrementCounters("Unique_lock", unique_lck, ITERATIONS, NUMTHREADS);
    incrementCounters("try_lock_for", try_lock_for, ITERATIONS, NUMTHREADS);
    std::println(
        "{}{:<15}{:>10}{}", GREEN, "  └─ retries", try_lock_for.get_failed_lock_count(), RESET);
    incrementCounters("try_lock_until", try_lock_until, ITERATIONS, NUMTHREADS);
    std::println(
        "{}{:<15}{:>10}{}", GREEN, "  └─ retries", try_lock_until.get_failed_lock_count(), RESET);

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
        std::println("\n\033[1;31m{}\033[0m", e.what());
        std::println("{}::at({}{}{}{}{}) > vector size:{}{}{}{}",
                     YELLOW,
                     RESET,
                     RED,
                     e.index,
                     RESET,
                     YELLOW,
                     RESET,
                     GREEN,
                     e.size,
                     RESET);
        std::println("{}{}::@Line:{}{}", YELLOW, e.location.file_name(), e.location.line(), RESET);
    }

    std::println("");

    std::thread rthr1(bad_factorial, 10);
    std::thread rthr2(bad_factorial, 11);

    rthr1.join();
    rthr2.join();
    return 0;
}
