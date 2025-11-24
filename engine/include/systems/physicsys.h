#pragma once
#include <espch.h>
#include "scene.h"


namespace engine {

    void physics(entt::registry& registry, float dt);
    int  aabb_check(Scene& scene, float dt);
}
