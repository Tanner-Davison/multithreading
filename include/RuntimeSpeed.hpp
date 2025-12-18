#pragma once
#include <chrono>
#include <print>
#include <string_view>
#include <unordered_map>

// Multiton Pattern (or keyed Singleton)
class RuntimeSpeed {
    inline static std::unordered_map<std::string, std::unique_ptr<RuntimeSpeed>> profilers;
    std::chrono::steady_clock::time_point                                        start;
    std::string_view                                                             label;

    RuntimeSpeed(std::string_view label = "Execution")
        : start(std::chrono::steady_clock::now())
        , label(label) {}

    RuntimeSpeed(const RuntimeSpeed&)            = delete;
    RuntimeSpeed& operator=(const RuntimeSpeed&) = delete;
    RuntimeSpeed(RuntimeSpeed&&)                 = delete;
    RuntimeSpeed& operator=(RuntimeSpeed&&)      = delete;

  public:
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
    static RuntimeSpeed& getProfiler(std::string_view name = "Execution") {
        std::string key(name);

        if (!profilers.contains(key)) {
            profilers[key] = std::unique_ptr<RuntimeSpeed>(new RuntimeSpeed(name));
        }
        return *profilers[key];
    }
};
