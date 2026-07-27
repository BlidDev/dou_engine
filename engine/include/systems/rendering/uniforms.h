#pragma once 
#include <espch.h>
#include "renderer.h"
#include "components/modelcomp.h"
#include "components/transform.h"
#include "components/camera.h"

namespace engine {

    void bind_and_clear_fb(Frambuffer& framebuffer, bool external_clear, SceneRenderData* s_render_data, int clear_flags);
    void set_ubos(entt::registry& registry, RenderData& data, SceneRenderData* s_render_data, CameraComp& viewer_camera, TransformComp& viewer_trans,const glm::vec2& view_size, const bool& external_clear);

    void send_lights(entt::registry &registry, RenderData &data);
    void send_material(Material &material);
}
