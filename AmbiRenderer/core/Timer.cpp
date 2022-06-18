#include "Timer.h"

void Timer::reset() noexcept
{
    m_old = std::chrono::high_resolution_clock::now();
}

float Timer::elapsed_micro() const noexcept
{
    return elapsed<std::chrono::microseconds>();
}

float Timer::elapsed_milliseconds() const noexcept
{
    return elapsed<std::chrono::milliseconds>();
}

float Timer::elapsed_seconds() const noexcept
{
    return elapsed<std::chrono::seconds>();
}

float Timer::elapsed_minutes() const noexcept
{
    return elapsed<std::chrono::minutes>();
}

template<typename Duration>
inline float Timer::elapsed() const noexcept
{
    using namespace std::chrono;
    return duration_cast<duration<float, Duration::period>>(high_resolution_clock::now()-m_old).count();
}