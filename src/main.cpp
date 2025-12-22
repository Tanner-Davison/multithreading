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
void run_demo();
}
namespace deadlock_example {
void run_philosophers();
}
namespace pre_seventeen_deadlock_example {
void run_demo();
void run_demo_using_defer();
} // namespace pre_seventeen_deadlock_example

int main() {
    RuntimeSpeed& mainloop = RuntimeSpeed::getProfiler("Main Thread");
    // thread_local_example::run_demo();
    // singleton_example::run_demo();
    // shared_mutex_example::run_demo();
    // recursive_mutex_example::run_bad_factorial();
    // double_checked_locking::run_demo();
    // deadlock_example::run_demo();
    // deadlock_example::run_philosophers();
    // pre_seventeen_deadlock_example::run_demo();
    pre_seventeen_deadlock_example::run_demo_using_defer();
    return 0;
}
