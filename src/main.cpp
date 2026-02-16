#include <RuntimeSpeed.hpp>
#include <print>

using namespace std::literals;

namespace shared_mutex_example {
void run_demo();
}
namespace singleton {
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
void run_with_deadlock(); // WARN: runs forever
} // namespace deadlock_example
namespace pre_seventeen_deadlock_example {
void run_demo();
void run_demo_using_defer();
void run_try_lock_demo();
} // namespace pre_seventeen_deadlock_example
namespace bank_example {
void deposit_and_check_balance();
} // namespace bank_example
namespace livelock_example {
void run_livelock_demo();
void run_livelock_eternal_conversation(); // WARN: runs forever
} // namespace livelock_example
namespace thread_coordination {
void run_demo();
} // namespace thread_coordination
namespace condition_variable_example {
void run_demo();
}
namespace condition_variable_practical {
void run_demo();
}
namespace future_and_promise {
void run_demo();
}

int main() {
    RuntimeSpeed::getProfiler();
    future_and_promise::run_demo();
    return 0;
}
