#include "entry_point.h"
#include "manager.h"

#define SOL_ALL_SAFETIES_ON 1

using namespace engine;

static int FRAME_CAP = 0;

int main() {

    glfwInit();

    Log::init();

    SceneManager manager;
    on_start(&manager);

    Scene* current = manager.get_current();

    double start = glfwGetTime();

    while (!current->should_close())
    {
        double now = glfwGetTime();
        double dt = now - start;
        if (manager.old) {
            manager.end_scene(manager.old);
            manager.old = nullptr;
        }
        if (manager.switched) { 
            current = manager.get_current();
            manager.switched = false;
            current->on_create();
        }

        double desired = (FRAME_CAP == 0) ? (0.0) : (1.0 / (float)FRAME_CAP);
        if (dt > desired) {
            current->on_update(dt);
            start = now;
        }
    }

    manager.end();

    on_end(&manager);

    
    return 0;
}


void engine::set_frame_cap(int frame_cap) {
    DU_ASSERT(frame_cap < 0, "Invalid frame cap ({}) given", frame_cap);
    FRAME_CAP = frame_cap;
}
