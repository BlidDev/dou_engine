#include "components/luascript.h"
#include "entry_point.h"
#include "manager.h"

#define SOL_ALL_SAFETIES_ON 1

using namespace engine;


int main() {

    glfwInit();

    Log::init();
    LuaManager::init();

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
            update_render_data(&manager, current);
        }
        current->on_update(dt);
        start = now;
    }

    on_end(&manager);

    manager.end();
    
    return 0;
}
