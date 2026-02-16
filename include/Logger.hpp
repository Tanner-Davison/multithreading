#pragma once
#include <mutex>
#include <print>
#include <string>
#include <string_view>
#include <vector>

/*
 * Thread-safe singleton logger.
 * Uses a mutex to protect the internal log vector since multiple
 * threads may write concurrently.
 */
class Logger {
  private:
    Logger() = default;
    std::vector<std::string> logs;
    mutable std::mutex       mtx;

  public:
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&)                 = delete;
    Logger& operator=(Logger&&)      = delete;

    static Logger& get_logger() {
        static Logger log;
        return log;
    }

    void log(std::string_view msg) {
        std::lock_guard<std::mutex> lock(mtx);
        logs.emplace_back(msg);
    }

    void read_latest_log() const {
        std::lock_guard<std::mutex> lock(mtx);
        if (!logs.empty()) {
            std::println("{}", logs.back());
        }
    }

    void read_all_logs() const {
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto& l : logs) {
            std::println("{}", l);
        }
    }
};
