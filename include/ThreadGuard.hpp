#pragma once
#include <iostream>
#include <thread>

class ThreadGuard {
    std::thread thr;

  public:
    explicit ThreadGuard(std::thread&& p_thr);

    ~ThreadGuard(); // joins thread if joinable()
                    // deleted copy operators to prevent copying

    ThreadGuard(const ThreadGuard&)            = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
};
