#include "components/camera.h"
#include "util.h"

namespace engine {
    void update_camera_target(CameraComp& camera, glm::vec3 position) {
        glm::vec3 delta = position - camera.last_pos; 
        camera.target += delta; 
        camera.last_pos = position;
    }

    void CameraComp::log() {
        EG_CORE_INFO("Camera:");
        EG_CORE_INFO("\tUp: ({}, {}, {})", up.x, up.y, up.z);
        EG_CORE_INFO("\tTarget: ({}, {}, {})", target.x, target.y, target.z);
        EG_CORE_INFO("\tFovy: {}", fovy);
        EG_CORE_INFO("\tProj: {}", (int)projection);
    }

    CameraBuilder::CameraBuilder() {
        camera = { 
            .target = (glm::vec3){ 0.0f, 0.0f, 1.0f },      // Camera looking at point
            .up = (glm::vec3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
            .fovy = 90.0f,                                // Camera field-of-view Y
            .projection = CameraProjection::Perspective,
            .last_pos = (glm::vec3){0.0f, 0.0f, 0.0f}
        };
    }

    CameraBuilder& CameraBuilder::target(glm::vec3 target) {
        camera.target = target;
        return *this;
    }

    CameraBuilder& CameraBuilder::up(glm::vec3 up) {
        camera.up = up;
        return *this;
    }

    CameraBuilder& CameraBuilder::fovy(float fovy) {
        camera.fovy = fovy;
        return *this;
    }

    CameraBuilder& CameraBuilder::projection(CameraProjection projection) {
        camera.projection = projection;
        return *this;
    }

    CameraComp CameraBuilder::build() {
        return camera;
    }


    glm::vec3 get_camera_dir(glm::vec3& target, glm::vec3& position) {
        glm::vec3 dir = glm::normalize(target - position);
        return dir;
    }

    glm::vec3 get_flat_forward(glm::vec3& target, glm::vec3& position) {
        glm::vec3 dir = glm::normalize(target - position);
        float yaw = atan2(dir.z, dir.x);

        glm::vec3 flat_forward = glm::vec3(cos(yaw), 0.0f, sin(yaw));

        return flat_forward;
    }
}
