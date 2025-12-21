#include <mutex>
#include <print>
#include <thread>
#include <vector>

using namespace std::literals;
namespace deadlock_example {
namespace {

/* 5 Philosophers sit down at a table each philosphers has a fork on each side of him
 * a single philospher can eat only if they can pick up both forks
 * if a philosopher picks up the fork on there right that prevents the philospher on there right
 * from picking up the fork to there left */

constexpr int         n_forks                   = 5;
constexpr int         n_philosophers            = n_forks;
constexpr std::string names[n_philosophers]     = {"A", "B", "C", "D", "E"};
int                   mouthfuls[n_philosophers] = {0};
constexpr auto        think_time                = 100ms;
constexpr auto        eat_time                  = 50ms;

std::mutex fork_mutex[n_forks];

void print(int n_id, const std::string& str, int forkno) {
    std::println("Philosopher {} {} {}", names[n_id], str, forkno);
}
void print(int n_id, const std::string& str) {
    std::println("Philosopher {} {}", names[n_id], str);
}
void dine(int n_philo) {
    int l_fork{n_philo};
    int r_fork{(n_philo + 1) % n_forks};

    std::println("{} is Thinking...", names[n_philo]);
    std::this_thread::sleep_for(think_time);
    std::scoped_lock lock_fork(fork_mutex[l_fork], fork_mutex[r_fork]);
    std::println("{} Picks up Left & Right fork #{} , #{}", names[n_philo], l_fork, r_fork);
    std::this_thread::sleep_for(think_time);
    std::println("{} is Eating ", names[n_philo]);
    std::println("{} Finishes Eating!", names[n_philo]);
    std::this_thread::sleep_for(think_time);

    mouthfuls[n_philo] += 1;
}

} // namespace

void run_philosophers() {
    std::vector<std::thread> philosphers;
    philosphers.reserve(n_philosophers);
    for (int i = 0; i < n_philosophers; ++i) {
        philosphers.emplace_back(dine, i);
    }
    for (auto& philo : philosphers) {
        philo.join();
    }
    for (int i = 0; i < n_philosophers; ++i) {
        std::println("{} was able to eat {} times.", names[i], mouthfuls[i]);
    }
}

} // namespace deadlock_example
