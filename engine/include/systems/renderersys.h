#pragma once
#include "entity.h"
#include "renderer.h"

namespace engine {
    void draw_to_camera(RenderData& data,glm::vec2 view_size, Entity& viewer, entt::registry& registry, SceneRenderData* s_render_data = nullptr, bool external_clear = false, uint32_t parent_fb = 0);
    void present_camera(Entity& viewer, Mesh& model);
}
