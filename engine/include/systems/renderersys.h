#pragma once
#include "entity.h"
#include "renderer.h"

namespace engine {
    void opengl_renderer(RenderData& data,glm::vec2 view_size, Entity viewer, entt::registry& registry, SceneRenderData* s_render_data = nullptr, bool external_clear = false);
}
