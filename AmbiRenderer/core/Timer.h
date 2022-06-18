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
    float elapsed_micro() const noexcept;
    float elapsed_milliseconds() const noexcept;
    float elapsed_seconds() const noexcept;
    float elapsed_minutes() const noexcept;

private:
    // 返回自上次计时以来经过的时间, 默认返回毫秒
    template<typename Duration>
    float elapsed() const noexcept;
};
