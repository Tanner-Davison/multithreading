#include "RuntimeSpeed.hpp"
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

int main() {
    RuntimeSpeed& mainloop = RuntimeSpeed::getProfiler("Main Thread");
    // thread_local_example::run_demo();
    // singleton_example::run_demo();
    // shared_mutex_example::run_demo();
    // recursive_mutex_example::run_bad_factorial();
    double_checked_locking::run_demo();
    return 0;
}
