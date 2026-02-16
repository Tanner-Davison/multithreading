#pragma once
#include <iostream>
#include <thread>
/*
 * Greate project to Write once but essentially just use jthread if you are able to write in c++20
 * or later
 * */
class ThreadGuard {
    std::thread thr;

  public:
    explicit ThreadGuard(std::thread&& p_thr);

    ~ThreadGuard(); // joins thread if joinable()

    // deleted copy operators to prevent copying
    ThreadGuard(const ThreadGuard&)            = delete;
    ThreadGuard& operator=(const ThreadGuard&) = delete;
};
