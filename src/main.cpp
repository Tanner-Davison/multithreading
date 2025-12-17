#include "AtomicCounter.hpp"
#include "IncrementCounters.hpp"
#include "MutexLocker.hpp"
#include "MutexUnique.hpp"
#include "TryLockFor.hpp"
#include "TryLockUntil.hpp"
#include "Vector.hpp"
#include <iostream>
#include <mutex>
#include <print>
#include <shared_mutex>
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

std::mutex        mut;
std::shared_mutex shmut;

int                 x{0};
std::pair<int, int> p(0, 0);

void write() {
    // std::lock_guard<std::mutex> lck_guard(mut);
    std::lock_guard<std::shared_mutex> lck_guard(shmut);
    ++p.first;
}
void read() {
    std::shared_lock lock_guard(shmut); // shared locking example
    std::this_thread::sleep_for(100ms);
    ++p.second;
}
int main() {
    // Vector my_vec;

    // const int ITERATIONS{100000};
    // const int NUMTHREADS{2};

    // AtomicCounter atomic_lck;
    // MutexLocker   lck_guard;
    // TryLockFor    try_lock_for;
    // MutexUnique   unique_lck;
    // TryLockUntil  try_lock_until;

    // std::println("{:<15} {:>10}", "Name:", "Count:");
    // IncrementCounters("Atomic_counter", atomic_lck, ITERATIONS, NUMTHREADS);
    // IncrementCounters("Lock_guard", lck_guard, ITERATIONS, NUMTHREADS);
    // IncrementCounters("Unique_lock", unique_lck, ITERATIONS, NUMTHREADS);
    // IncrementCounters("try_lock_for", try_lock_for, ITERATIONS, NUMTHREADS);
    // std::println(
    //     "{}{:<15}{:>10}{}", GREEN, "  └─ retries", try_lock_for.get_failed_lock_count(), RESET);
    // IncrementCounters("try_lock_until", try_lock_until, ITERATIONS, NUMTHREADS);
    // std::println(
    //     "{}{:<15}{:>10}{}", GREEN, "  └─ retries", try_lock_until.get_failed_lock_count(),
    //     RESET);
    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> threads;
    for (int i = 0; i < 20; ++i) {
        threads.push_back(std::thread(read));
    }

    for (auto& thr : threads) {
        thr.join();
    }

    auto end      = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Total execution time: " << duration.count() << "ms\n";
    return 0;
}
