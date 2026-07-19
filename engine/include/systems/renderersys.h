#pragma once
#include "entity.h"
#include "renderer.h"

namespace engine {
    void draw_to_camera(RenderData& data,glm::vec2 view_size, Entity& viewer, entt::registry& registry, SceneRenderData* s_render_data = nullptr, bool external_clear = false);
    void present_camera(Entity& viewer, Mesh& mesh);

    struct StateCounter {

        StateCounter(size_t obj_num) { objects_num = obj_num; }
        void add_global() {global++;}
        void add_call() {draw_calls++;}
        void add_shader() {shader++;}
        void add_vao() {vao++;}
        void add_texture() {texture++;}

        void summery() {
            DU_CORE_INFO("Frame Summery:");
            DU_CORE_INFO("\tNumber of objects: {}", objects_num);
            DU_CORE_INFO("\tDraw calls: {}", draw_calls);
            DU_CORE_INFO("\tGlobal changes: {}", global);
            DU_CORE_INFO("\tVao changes: {}", vao);
            DU_CORE_INFO("\tShader changes: {}", shader);
            DU_CORE_INFO("\tTexture changes: {}", texture);
        }
    private:
        size_t objects_num;
        size_t global = 0;
        size_t draw_calls = 0;
        size_t shader = 0;
        size_t vao = 0;
        size_t texture = 0;
    };
}
