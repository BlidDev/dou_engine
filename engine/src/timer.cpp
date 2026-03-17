#include "timer.h"


namespace engine {
    Timer::Timer() {
        reset();
    }

    void Timer::reset() {
        inner = std::chrono::high_resolution_clock::now();
    }

    float Timer::elapsed_millis() {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - inner).count();
    }

    ContainedTimer::ContainedTimer(float millis) : millis(millis), Timer() {
    }

    void ContainedTimer::set_delay(float millis) {
        this->millis = millis;
    }

    bool ContainedTimer::allow_and_reset() {
        auto now = std::chrono::high_resolution_clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - inner).count();

        bool reset = elapsed >= millis;
        if (reset)
            inner = now;
        return reset;
    }
}
