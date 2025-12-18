#pragma once
#include <chrono>
#include <print>
#include <string_view>

class RuntimeSpeed {
    std::chrono::steady_clock::time_point start;
    std::string_view                      label;

  public:
    RuntimeSpeed(std::string_view label = "Execution")
        : start(std::chrono::steady_clock::now())
        , label(label) {}

    ~RuntimeSpeed() {
        auto end      = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::println("{} Total time: {}ms", label, duration.count());
    }

    auto elapsed_time() const {
        auto now     = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
        return elapsed;
    }
};
