#include "bad_factorial.hpp"
#include <print>

std::recursive_mutex rmut; // Define the mutex

int bad_factorial(int n) {
    if (n <= 1) {
        std::println("Returning {}", 1);
        return 1;
    }
    std::lock_guard<std::recursive_mutex> lck(rmut);
    int                                   retval = n * bad_factorial(n - 1);
    std::println("Returning {}", retval);
    return retval;
}
