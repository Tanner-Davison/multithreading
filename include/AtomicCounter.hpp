
#pragma once
#include <atomic>

class AtomicCounter {
  private:
    mutable std::atomic<int> count{0};

  public:
    void increment() {
        ++count;
    }
    int get() const {
        return count.load();
    }
};
