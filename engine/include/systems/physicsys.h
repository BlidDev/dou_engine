#pragma once
#include <espch.h>
#include "scene.h"

namespace engine {
    int physics(Scene* scene, float dt);
    int fixed_physics(Scene* scene, size_t target_fps, float dt);
}
