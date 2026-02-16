#pragma once
#include <iostream>
#include <thread>
/*
 * LEARNING EXERCISE: Hand-rolled RAII thread guard.
 *
 * This is essentially what std::jthread (C++20) does for you automatically.
 * std::jthread also supports cooperative cancellation via std::stop_token.
 *
 * Since this project uses C++23, prefer std::jthread in real code.
 * Keeping this as a reference for understanding RAII thread lifetime management.
 */
class ThreadGuard {
    std::thread thr;

  public:
    explicit ThreadGuard(std::thread&& p_thr);

    ~ThreadGuard(); // joins thread if joinable()

    // deleted copy operators to prevent copying
    ThreadGuard(const ThreadGuard&)            = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
};
