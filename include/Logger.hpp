#pragma once
#include <print>
#include <string>
#include <string_view>
#include <vector>

class Logger {
  private:
    Logger() = default;
    std::vector<std::string> logs;

  public:
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&)                 = delete;
    Logger& operator=(Logger&&)      = delete;

    static Logger& get_logger(std::string_view logs) {
        static Logger log;
        return log;
    };
    std::string_view set_state_log(std::string_view msg) {
        logs.emplace_back(msg);
        return logs.back();
    }
    void read_latest_log() const {
        std::println("{}", logs.back());
    }
    void read_all_logs() const {
        for (auto& l : logs) {
            std::println("{}", l);
        }
    }
};
