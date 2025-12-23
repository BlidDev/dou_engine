#pragma once
#include "scene.h"

namespace engine {
    void actions_init(Scene* scene);
    void actions_update(Scene* scene, float dt);
    void actions_end(Scene* scene);
}

