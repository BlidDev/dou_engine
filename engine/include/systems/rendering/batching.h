#pragma once
#include <espch.h>
#include "components/modelcomp.h"


namespace engine {

    struct BatchCommand {
        uint64_t sort_key;

        size_t       layer;
        unsigned int vao;
        unsigned int shader;
        unsigned int texture;

        glm::mat4 model;
        glm::mat3 normal;
        ModelComp* model_cmp;
    };

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

    uint64_t generate_sort_key(unsigned int layer, unsigned int vao, unsigned int shader, unsigned int texture, unsigned int misc = 0);
}
