#pragma once
#include <iostream>
#include <thread>

class ThreadGaurd {
  std::thread thr;

public:
  explicit ThreadGaurd(std::thread &&p_thr);

  ~ThreadGaurd(); // joins thread if joinable()
                  // deleted copy operators to prevent copying

  ThreadGaurd(const ThreadGaurd &) = delete;
  ThreadGaurd &operator=(const ThreadGaurd &) = delete;
};
