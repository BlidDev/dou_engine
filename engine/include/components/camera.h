#pragma once
#include "shader.h"
#include "window.h"
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
        CameraComp();
        glm::vec3 up;
        float fovy;
        CameraProjection projection_mode;
        float max_distance;

        Frambuffer framebuffer;
        Shader present_shader;

        glm::mat4 get_projection(glm::vec2 view_size);
        glm::mat4 get_view(glm::vec3 view_pos);

        constexpr float pitch() const {return f_pitch;}
        constexpr float yaw() const {return f_yaw;}

        float set_pitch(float pitch, float max = 180.0f);
        float set_yaw(float pitch, float max = 360.0f);

        void log();
    private:
        float f_pitch;
        float f_yaw;
    };

    struct CameraBuilder {

        CameraBuilder();


        CameraBuilder& pitch(float pitch);
        CameraBuilder& yaw(float yaw);
        CameraBuilder& up(glm::vec3 up);
        CameraBuilder& fovy(float fovy);
        CameraBuilder& projection_mode(CameraProjection projection);
        CameraBuilder& max_distance(float distance);
        CameraBuilder& framebuffer_size(size_t w, size_t h);
        CameraBuilder& present_shader(Shader& shader);

        CameraComp build();
    private:
        CameraComp camera;
    };


    glm::vec3 get_camera_dir(const CameraComp& camera);
    glm::vec3 get_camera_target(const CameraComp& camera, const glm::vec3& position);

    glm::vec3 get_camera_right(const CameraComp& camera);
    glm::vec3 get_camera_forward(const CameraComp& camera);
    glm::vec3 get_camera_flat_forward(const CameraComp& camera);


    void rescale_camera_to_window(CameraComp& camera, Window& window);

    void camera_pitch(CameraComp& camera, float delta, bool lock);
    void camera_yaw(CameraComp& camera, float delta);
    void handle_mouse_delta(CameraComp& camera, glm::vec2 delta, bool lock);
}
