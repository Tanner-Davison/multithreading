#include "ThreadGuard.hpp"

ThreadGuard::ThreadGuard(std::thread&& p_thr)
    : thr(std::move(p_thr)) {

    };
ThreadGuard::~ThreadGuard() {
    if (thr.joinable()) {
        std::cout << "\nJoining Thread before ThreadGuard goes out of scope!\n";
        thr.join();
    }
};
