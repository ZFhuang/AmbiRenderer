#include "Timer.h"

void Timer::reset() noexcept
{
    m_old = std::chrono::high_resolution_clock::now();
}

int64_t Timer::elapsed_micro() const noexcept
{
    return elapsed<std::chrono::microseconds>();
}

int64_t Timer::elapsed_milliseconds() const noexcept
{
    return elapsed<std::chrono::milliseconds>();
}

int64_t Timer::elapsed_seconds() const noexcept
{
    return elapsed<std::chrono::seconds>();
}

int64_t Timer::elapsed_minutes() const noexcept
{
    return elapsed<std::chrono::minutes>();
}

template<typename Duration>
inline int64_t Timer::elapsed() const noexcept
{
    using namespace std::chrono;
    return duration_cast<Duration>(high_resolution_clock::now()-m_old).count();
}