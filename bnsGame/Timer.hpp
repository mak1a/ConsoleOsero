#pragma once
#include <chrono>

class Timer {
public:
    using ms = std::chrono::milliseconds;
    using s = std::chrono::seconds;

private:
    std::chrono::time_point<std::chrono::system_clock> m_begin;
    std::chrono::time_point<std::chrono::system_clock> m_now;

    bool m_isActive;

    template <typename ClockType>
    long long GetTime() const {
        if (!m_isActive) {
            auto time = std::chrono::duration_cast<ClockType>(m_now - m_begin);
            return time.count();
        }

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::duration_cast<ClockType>(now - m_begin);
        return time.count();
    }

public:
    Timer(const bool active_) noexcept
    : m_isActive(active_) 
    , m_begin(std::chrono::system_clock::now())
    , m_now(std::chrono::system_clock::now()) {}

    [[nodiscard]] bool IsActive() const noexcept {
        return m_isActive;
    }

    void Start() {
        if (m_isActive) {
            return;
        }
        m_begin = std::chrono::system_clock::now();
        m_isActive = true;
    }

    void Restart() {
        Stop();
        Start();
    }

    void Stop() {
        m_isActive = false;
        m_now = std::chrono::system_clock::now();
    }

    long long GetMilliSeconds() const {
        return GetTime<ms>();
    }

    long long GetSeconds() const {
        return GetTime<s>();
    }
};