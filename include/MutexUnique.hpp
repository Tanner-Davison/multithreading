#include <mutex>

class MutexUnique {
    mutable int        count{0};
    mutable std::mutex u_mtx;

  public:
    void increment() {
        std::unique_lock<std::mutex> u_lock(u_mtx);
        ++count;
    }
    int get() const {
        std::unique_lock<std::mutex> u_lock(u_mtx);
        return count;
    }
};
