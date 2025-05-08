#include <espch.h>

namespace engine {
    class Timer {
    public:
        Timer();
        void reset();
        float elapsed_millis();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> inner;
    };
}
