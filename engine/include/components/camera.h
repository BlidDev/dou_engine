#pragma once
#include "shader.h"
#include <espch.h>

namespace engine {
    
    struct Frambuffer {
        uint32_t handler;
        uint32_t rbo;
        uint32_t texture;
        glm::vec2 last_scale;

        Frambuffer() {
            handler = 0; rbo = 0;
            texture = 0;
            last_scale = glm::vec2(0.0f);
        }

        operator uint32_t() const {
            DU_ASSERT(handler == 0, "Trying to retreive invalid framework handler");
            return handler;
        }

        operator bool() const {
            return handler != 0 && rbo != 0 && texture != 0;
        }


        void free(bool delete_texture = false) {
            if (delete_texture)
                glDeleteTextures(1, &texture);
            glDeleteRenderbuffers(1, &rbo);
            DU_CORE_DEBUG_TRACE("Freed framebuffer {}", handler);
            glDeleteFramebuffers(1, &handler);
        }
    };

    void make_framebuffer(Frambuffer& fb, size_t w, size_t h);
    void rescale_framebuffer(Frambuffer& fb, size_t w, size_t h);

    enum CameraProjection {
        Perspective,
        Orthographic
    };

    struct CameraComp {
        glm::vec3 target;
        glm::vec3 up;
        float fovy;
        CameraProjection projection;
        glm::vec3 last_pos;
        float max_distance;

        Frambuffer framebuffer;
        Shader present_shader;
        void log();
    };

    void update_camera_target(CameraComp& camera, glm::vec3 position);

    glm::mat4 get_camera_proj(CameraComp& camera, glm::vec2 view_size);
    glm::mat4 get_camera_view(CameraComp& camera, glm::vec3 position);

    struct CameraBuilder {
        CameraBuilder();

        CameraBuilder& target(glm::vec3 target);
        CameraBuilder& up(glm::vec3 up);
        CameraBuilder& fovy(float fovy);
        CameraBuilder& projection(CameraProjection projection);
        CameraBuilder& max_distance(float distance);
        CameraBuilder& framebuffer_size(size_t w, size_t h);
        CameraBuilder& present_shader(Shader& shader);

        CameraComp build();
    private:
        CameraComp camera;
    };


    glm::vec3 get_flat_forward(glm::vec3& target, glm::vec3& position);
    glm::vec3 get_camera_dir(glm::vec3& target, glm::vec3& position);
}
