
/**
 * @file RuntimeSpeed.hpp
 * @author Tanner Davison
 * @brief Multiton pattern profiler for measuring execution time of code sections
 * @copyright Copyright (c) 2025 Tanner Davison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include <chrono>
#include <mutex>
#include <print>
#include <string_view>
#include <unordered_map>

// Multiton Pattern
class RuntimeSpeed {
    inline static std::unordered_map<std::string, std::unique_ptr<RuntimeSpeed>> profilers;
    inline static std::mutex                                                     profilers_mutex;

    std::chrono::steady_clock::time_point start;
    std::string_view                      label;

    RuntimeSpeed(std::string_view label = "Execution")
        : start(std::chrono::steady_clock::now())
        , label(label) {}

    /*deleting all copy and assaginment overloads */
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
        std::lock_guard<std::mutex> lock(profilers_mutex);
        std::string                 key(name);
        if (!profilers.contains(key)) {
            profilers[key] = std::unique_ptr<RuntimeSpeed>(new RuntimeSpeed(name));
        }
        return *profilers[key];
    }
};
