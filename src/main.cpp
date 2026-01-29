#include "RuntimeSpeed.hpp"
#include <print>

namespace shared_mutex_example {
void run_demo();
}
namespace singleton_example {
void run_demo();
}
namespace thread_local_example {
void run_demo();
}
namespace recursive_mutex_example {
int run_bad_factorial();
}
namespace double_checked_locking {
void run_safer_call_once();
void run_double_check_demo();
} // namespace double_checked_locking
namespace deadlock_example {
void run_philosophers();
void run_with_deadlock();
} // namespace deadlock_example
namespace pre_seventeen_deadlock_example {
void run_demo();
void run_demo_using_defer();
void run_try_lock_demo();
} // namespace pre_seventeen_deadlock_example

int main() {
    RuntimeSpeed::getProfiler("Main Thread");
    // thread_local_example::run_demo();
    // singleton_example::run_demo();
    // shared_mutex_example::run_demo();
    // recursive_mutex_example::run_bad_factorial();
    // double_checked_locking::run_double_check_demo();
    // double_checked_locking::run_double_check_demo();
    deadlock_example::run_philosophers();
    // deadlock_example::run_with_deadlock();
    // pre_seventeen_deadlock_example::run_demo();
    // pre_seventeen_deadlock_example::run_demo_using_defer();
    // pre_seventeen_deadlock_example::run_try_lock_demo();

    return 0;
}
