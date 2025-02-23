#include "components/camera.h"

namespace engine {
    CameraBuilder::CameraBuilder() {
        camera = { 
            .target = (glm::vec3){ 0.0f, 0.0f, 1.0f },      // Camera looking at point
            .up = (glm::vec3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
            .fovy = 90.0f,                                // Camera field-of-view Y
            .projection = CameraProjection::Perspective
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

    Camera CameraBuilder::build() {
        return camera;
    }

}
