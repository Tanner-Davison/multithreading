#include <mutex>
#include <print>
#include <string>
#include <thread>
#include <vector>

namespace bank_example {
namespace {

class Account {
  private:
    std::mutex balance_mutex_;
    std::mutex transaction_history_mutex_;

    std::vector<std::string> transaction_history_;
    float                    balance_{0.0};

  public:
    void deposit(double amount) {
        {
            std::lock_guard<std::mutex> lock(balance_mutex_);
            balance_ += amount; // only locks balance, history still accessible
        }
        {
            std::lock_guard<std::mutex> lock(transaction_history_mutex_);
            auto                        now = std::chrono::system_clock::now();
            auto                        time_t_now{std::chrono::system_clock::to_time_t(now)};
            char                        buffer[100];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time_t_now));
            transaction_history_.push_back(std::format("\n {} Deposited: ${:.2f}", buffer, amount));
        }
    }
    void printHistory() {
        std::lock_guard<std::mutex> lock(transaction_history_mutex_);
        for (const auto& tx : transaction_history_) {
            std::print("{}", tx);
        }
    }
    void getBalance() {
        std::lock_guard<std::mutex> lock(balance_mutex_);

        std::println("\nTotal Deposited:\t\t ${:.2f}", balance_);
    }
};
} // namespace

void deposit_and_check_balance() {
    Account bank;

    std::vector<std::thread> threads;
    threads.reserve(50);

    for (int i = 0; i < 50; ++i) {
        threads.emplace_back([&bank, i]() { bank.deposit((i + 0.50)); });
    }
    for (auto& t : threads) {
        t.join();
    }

    bank.printHistory();
    bank.getBalance();
}
} // namespace bank_example
