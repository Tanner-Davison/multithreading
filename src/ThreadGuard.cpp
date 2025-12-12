#include "ThreadGuard.hpp"

ThreadGuard::ThreadGuard(std::thread&& p_thr)
    : thr(std::move(p_thr)) {

    };
ThreadGuard::~ThreadGuard() {
    if (thr.joinable()) {
        std::cout << "\n---ThreadGuard Destructor Called Join()---\n";
        thr.join();
    }
};
