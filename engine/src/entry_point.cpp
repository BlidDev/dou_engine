#include "engine.h"

#define SOL_ALL_SAFETIES_ON 1
#include <sol.hpp>

using namespace engine;


int main() {

    Log::init();
    LuaManager::init();

    SceneManager manager;
    on_start(&manager);

    Scene* current = manager.get_current();

    while (!current->should_close())
    {
        if (manager.switched) { 
            current = manager.get_current();
            manager.switched = false;
            current->on_create();
        }
        current->on_update(GetFrameTime());
    }


    manager.end();
    
    return 0;
}
