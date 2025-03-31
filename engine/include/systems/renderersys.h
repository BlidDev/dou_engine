#pragma once
#include "entity.h"

namespace engine {
    void opengl_renderer(glm::vec2 view_size, Entity viewer, entt::registry& registry);
}
