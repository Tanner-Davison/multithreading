#include <mutex>
#include <print>
#include <thread>
#include <vector>

using namespace std::literals;
namespace deadlock_example {
namespace {

/* DEADLOCK PREVENTION EXAMPLE
 *
 * This demonstrates the Dining Philosophers problem and shows how to
 * PREVENT deadlock using std::scoped_lock, which atomically acquires
 * multiple mutexes to avoid the classic deadlock scenario.
 *
 * Classic deadlock occurs when:
 * - Each philosopher grabs their left fork first
 * - Then tries to grab their right fork
 * - All 5 philosophers grab left forks simultaneously → DEADLOCK
 *
 * Prevention: std::scoped_lock acquires BOTH mutexes atomically,
 * preventing the circular wait condition that causes deadlock.
 */
constexpr int         n_forks                   = 5;
constexpr int         n_philosophers            = n_forks;
constexpr std::string names[n_philosophers]     = {"A", "B", "C", "D", "E"};
int                   mouthfuls[n_philosophers] = {0};
constexpr auto        think_time                = 100ms;
constexpr auto        eat_time                  = 50ms;

std::mutex fork_mutex[n_forks];

void print(int n_id, const std::string& str, int forkno, int forknotwo) {
    std::println(
        "Philosopher {} {} {}, {} and starts eating!", names[n_id], str, forkno, forknotwo);
}
void print(int n_id, const std::string& str) {
    std::println("Philosopher {} {}", names[n_id], str);
}
void dine(int n_philo) {
    bool selfish_starter = false;
    int  l_fork{n_philo};
    int  r_fork{(n_philo + 1) % n_forks};

    print(n_philo, "is thinking..");
    std::this_thread::sleep_for(think_time);
    std::scoped_lock lock_fork(fork_mutex[l_fork], fork_mutex[r_fork]);
    if (!selfish_starter) {
        selfish_starter = true;
        print(n_philo, "Selfishly starts first & Picks up forks ", l_fork, r_fork);
    } else {
        print(n_philo, "Picks up both forks numbered ", l_fork, r_fork);
    }
    std::this_thread::sleep_for(eat_time);
    print(n_philo, "Finished eating");
    std::this_thread::sleep_for(think_time);

    mouthfuls[n_philo] += 1;
}

} // namespace

void run_philosophers() {
    std::vector<std::thread> table;
    table.reserve(n_philosophers);
    for (int i = 0; i < n_philosophers; ++i) {
        table.emplace_back(dine, i);
    }
    for (auto& philosophers : table) {
        philosophers.join();
    }
    for (int i = 0; i < n_philosophers; ++i) {
        std::println("{} was able to eat {} times.", names[i], mouthfuls[i]);
    }
}

} // namespace deadlock_example
