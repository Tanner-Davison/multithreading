#pragma once
#include <print>
#include <string>
#include <thread>
#include <vector>

template <typename ThreadCounters>
void IncrementCounters(const std::string& name,
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
    }
    for (auto& t : threads) {
        t.join();
    }
    std::println("{:<15} {:>10}", name, counter.get());
}
