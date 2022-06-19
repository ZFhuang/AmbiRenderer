#pragma once
#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_old;

public:
    Timer() noexcept : m_old(std::chrono::high_resolution_clock::now()){}

    // 重置计时器
    void reset() noexcept;

public:
    int64_t elapsed_micro() const noexcept;
    int64_t elapsed_milliseconds() const noexcept;
    int64_t elapsed_seconds() const noexcept;
    int64_t elapsed_minutes() const noexcept;

private:
    // 返回自上次计时以来经过的时间, 默认返回毫秒
    template<typename Duration>
    int64_t elapsed() const noexcept;
};
