#pragma once

#include <atomic>
#include <shared_mutex>
class Singleton {
  private:
    Singleton() = default;
    inline static std::atomic<int> access_counter;
    std::shared_mutex              shmut;

  public:
    Singleton(const Singleton&)            = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&)                 = delete;
    Singleton& operator=(Singleton&&)      = delete;

    static Singleton& get_instance() {
        // only the first thread exexutes this instance
        static Singleton instance;
        ++access_counter;
        return instance;
    }
    static int get_access_count() {
        return access_counter.load();
    }
};
