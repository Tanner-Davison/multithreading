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

int main() {
    RuntimeSpeed& mainloop = RuntimeSpeed::getProfiler("Main Thread");
    thread_local_example::run_demo();
    singleton_example::run_demo();
    shared_mutex_example::run_demo();
    return 0;
}
