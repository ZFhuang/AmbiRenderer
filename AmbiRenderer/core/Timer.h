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
    float elapsed_micro() const noexcept;
    float elapsed_milliseconds() const noexcept;
    float elapsed_seconds() const noexcept;
    float elapsed_minutes() const noexcept;

private:
    // �������ϴμ�ʱ����������ʱ��, Ĭ�Ϸ��غ���
    template<typename Duration>
    float elapsed() const noexcept;
};
