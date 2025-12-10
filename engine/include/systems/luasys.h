#pragma once
#include "scene.h"

namespace engine {
    void lua_action_init(Scene* scene);
    void lua_action_update(Scene* scene, float dt);
    void lua_action_end(Scene* scene);

}
