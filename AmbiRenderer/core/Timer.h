#pragma once
#include <chrono>

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_old;

public:
    Timer() noexcept : m_old(std::chrono::high_resolution_clock::now()){}

    // ���ü�ʱ��
    void reset() noexcept;

public:
    int64_t elapsed_micro() const noexcept;
    int64_t elapsed_milliseconds() const noexcept;
    int64_t elapsed_seconds() const noexcept;
    int64_t elapsed_minutes() const noexcept;

private:
    // �������ϴμ�ʱ����������ʱ��, Ĭ�Ϸ��غ���
    template<typename Duration>
    int64_t elapsed() const noexcept;
};
