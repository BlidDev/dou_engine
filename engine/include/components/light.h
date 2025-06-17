#pragma once
#include <espch.h>


namespace engine {
    struct LightComp{
        LightComp() { color = {1.0f, 1.0f, 1.0f}; strength = 1.0f; };
        glm::vec3 color;
        float strength;
    };
}
