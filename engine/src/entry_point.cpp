#include <engine.h>
#include <GLFW/glfw3.h>

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
        if (manager.switched) { 
            current = manager.get_current();
            manager.switched = false;
            current->on_create();
        }
        current->on_update(dt);
        start = now;
    }


    manager.end();
    
    return 0;
}
