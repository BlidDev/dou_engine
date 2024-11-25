#pragma once
#include <espch.h>

namespace engine {
    struct CameraBuilder {
        CameraBuilder();

        CameraBuilder& position(Vector3 position);
        CameraBuilder& target(Vector3 target);
        CameraBuilder& up(Vector3 up);
        CameraBuilder& fovy(float fovy);
        CameraBuilder& projection(CameraProjection projection);

        Camera build();
    private:
        Camera camera;
    };

}
