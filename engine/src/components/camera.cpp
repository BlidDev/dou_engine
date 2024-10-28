#include "components/camera.h"

namespace engine {
    CameraBuilder::CameraBuilder() {
        camera = { 
            .position = (Vector3){ 0.0f, 0.0f, 0.0f },    // Camera positionl
            .target = (Vector3){ 0.0f, 0.0f, 1.0f },      // Camera looking at point
            .up = (Vector3){ 0.0f, 1.0f, 0.0f },          // Camera up vector (rotation towards target)
            .fovy = 90.0f,                                // Camera field-of-view Y
            .projection = CAMERA_PERSPECTIVE
        };
    }

    CameraBuilder& CameraBuilder::position(Vector3 position) {
        camera.position = position;
        return *this;
    }

    CameraBuilder& CameraBuilder::target(Vector3 target) {
        camera.target = target;
        return *this;
    }

    CameraBuilder& CameraBuilder::up(Vector3 up) {
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
