#include "ThreadGaurd.hpp"

ThreadGaurd::ThreadGaurd(std::thread &&p_thr)
    : thr(std::move(p_thr)) {

      };
ThreadGaurd::~ThreadGaurd() {
  if (thr.joinable()) {
    thr.join();
  }
};
