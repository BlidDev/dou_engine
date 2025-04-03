#pragma once
#include <espch.h>

namespace engine {
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
        CameraComp build();
    private:
        CameraComp camera;
    };


    glm::vec3 get_flat_forward(glm::vec3& target, glm::vec3& position);
}
