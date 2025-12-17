#include "AtomicCounter.hpp"
#include "IncrementCounters.hpp"
#include "MutexLocker.hpp"
#include "MutexUnique.hpp"
#include "ThreadGuard.hpp"
#include "TryLockFor.hpp"
#include "TryLockUntil.hpp"
#include "Vector.hpp"
#include "bad_factorial.hpp"
#include <print>
#include <thread>
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

int main() {
    Vector my_vec;

    const int ITERATIONS{100000};
    const int NUMTHREADS{2};

    AtomicCounter atomic_lck;
    MutexLocker   lck_guard;
    TryLockFor    try_lock_for;
    MutexUnique   unique_lck;
    TryLockUntil  try_lock_until;

    std::println("{:<15} {:>10}", "Name", "Count");
    IncrementCounters("Atomic_counter", atomic_lck, ITERATIONS, NUMTHREADS);
    IncrementCounters("Lock_guard", lck_guard, ITERATIONS, NUMTHREADS);
    IncrementCounters("Unique_lock", unique_lck, ITERATIONS, NUMTHREADS);
    IncrementCounters("try_lock_for", try_lock_for, ITERATIONS, NUMTHREADS);
    std::println(
        "{}{:<15}{:>10}{}", GREEN, "  └─ retries", try_lock_for.get_failed_lock_count(), RESET);
    IncrementCounters("try_lock_until", try_lock_until, ITERATIONS, NUMTHREADS);
    std::println(
        "{}{:<15}{:>10}{}", GREEN, "  └─ retries", try_lock_until.get_failed_lock_count(), RESET);

    // std::thread test(bad_factorial, 11);
    // std::thread test2(bad_factorial, 12);
    // test.join();
    // test2.join();
    return 0;
}
