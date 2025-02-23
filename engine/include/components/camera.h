#pragma once
#include <espch.h>
#include <glm/glm.hpp>

namespace engine {
    enum CameraProjection {
        Perspective,
        Orthographic
    };

    struct Camera {
        glm::vec3 target;
        glm::vec3 up;
        float fovy;
        CameraProjection projection;
    };

    struct CameraBuilder {
        CameraBuilder();

        CameraBuilder& target(glm::vec3 target);
        CameraBuilder& up(glm::vec3 up);
        CameraBuilder& fovy(float fovy);
        CameraBuilder& projection(CameraProjection projection);
        Camera build();
    private:
        Camera camera;
    };

}
