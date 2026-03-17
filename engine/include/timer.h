#include <chrono>

namespace engine {
    class Timer {
    public:
        Timer();
        void reset();
        float elapsed_millis();
    protected:
        std::chrono::time_point<std::chrono::high_resolution_clock> inner;
    };

    class ContainedTimer : public Timer {
    public:
        ContainedTimer() = delete;
        ContainedTimer(float millis);

        void set_delay(float millis);
        bool allow_and_reset();
    private:
        float millis;
    };
}
