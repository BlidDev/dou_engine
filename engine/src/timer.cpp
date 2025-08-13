#include "timer.h"
#include <chrono>


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

}
