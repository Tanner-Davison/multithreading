#include <algorithm>
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

std::atomic<bool>                selfish_starter{false};
constexpr int                    n_forks                   = 5;
constexpr int                    n_philosophers            = n_forks;
constexpr std::string_view       names[n_philosophers]     = {"A", "B", "C", "D", "E"};
constexpr auto                   think_time                = 100ms;
constexpr auto                   eat_time                  = 50ms;
int                              mouthfuls[n_philosophers] = {0};
std::vector<std::pair<int, int>> finished_positions;
constexpr std::string_view       ordinals[] = {"", "1st", "2nd", "3rd", "4th", "5th"};
int                              position{0};

std::mutex fork_mutex[n_forks];
std::mutex position_mutex;

void print(int n_id, const std::string& str, int forkno, int forknotwo) {
    std::println(
        "Philosopher {} {} {}, {} and starts eating!", names[n_id], str, forkno, forknotwo);
}
void print(int n_id, const std::string& str) {
    std::println("Philosopher {} {}", names[n_id], str);
}
void see_results(int n_id, const std::string& str, int mouthfuls, int position) {
    std::println("{} {} {} time{} finished in {} place.",
                 names[n_id],
                 str,
                 mouthfuls,
                 mouthfuls == 1 ? "" : "s",
                 ordinals[position]);
}
void dine(int n_philo, bool showDeadLock = false) {
    int left_fork{n_philo};
    int right_fork{(n_philo + 1) % n_forks};
    print(n_philo, "is thinking..");
    std::this_thread::sleep_for(think_time);

    if (showDeadLock) { // DEADLOCK EXAMPLE
        std::unique_lock lock_left(fork_mutex[left_fork]);
        std::this_thread::sleep_for(10ms); // Give others time to grab their left fork
        std::unique_lock lock_right(fork_mutex[right_fork]); // DEADLOCK HERE

        print(n_philo, "Picks up forks ", left_fork, right_fork);
        std::this_thread::sleep_for(eat_time);
        print(n_philo, "Finished eating");
        // locks release here
    } else {
        // Prevent deadlock with --atomic acquisition--
        std::scoped_lock lock_fork(fork_mutex[left_fork], fork_mutex[right_fork]);

        if (!selfish_starter.exchange(true)) {
            print(n_philo, "Selfishly starts first & Picks up forks ", left_fork, right_fork);
        } else {
            print(n_philo, "Picks up both forks numbered ", left_fork, right_fork);
        }
        std::this_thread::sleep_for(eat_time);
        print(n_philo, "Finished eating");
        // lock releases here
    }

    {
        std::lock_guard lock(position_mutex);
        finished_positions.emplace_back(n_philo, ++position);
        mouthfuls[n_philo] += 1;
    }

    std::this_thread::sleep_for(think_time);
}
} // namespace

// 2 AVAIABLE FUNCTIONS TO CALL FROM MAIN
void run_philosophers() {
    std::vector<std::thread> table;
    table.reserve(n_philosophers);
    for (int i = 0; i < n_philosophers; ++i) {
        table.emplace_back(dine, i, false);
    }
    for (auto& philosophers : table) {
        philosophers.join();
    }
    // sort before printing to see from A-E
    std::sort(finished_positions.begin(), finished_positions.end());
    for (const auto& [philo, position] : finished_positions) {
        see_results(philo, "ate", mouthfuls[philo], position);
    }
}
//  WARNING: This function will deadlock and hang forever!
//  Use Ctrl+C to terminate if you run this.
void run_with_deadlock() {
    std::vector<std::thread> table;
    table.reserve(n_philosophers);
    for (int i = 0; i < n_philosophers; ++i) {
        table.emplace_back(dine, i, true);
    }
    for (auto& philosophers : table) {
        philosophers.join();
    }
    // sort before printing to see from A-E
    std::sort(finished_positions.begin(), finished_positions.end());
    for (const auto& [philo, position] : finished_positions) {
        see_results(philo, "ate", mouthfuls[philo], position);
    }
}

} // namespace deadlock_example
