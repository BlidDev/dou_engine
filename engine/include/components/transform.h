#pragma once
#include <espch.h>
#include <glm/glm.hpp>


namespace engine {

    struct TransformComp {
        glm::vec3 position;
        glm::vec3 size;

        TransformComp(glm::vec3 position = {0.0f, 0.0f, 0.0f},glm::vec3 size = {1.0f, 1.0f, 1.0f}) {
            this->position = position;
            this->size = size;
        }
        //glm::vec3 rotation;
    };

    struct TransformBuilder {
        TransformBuilder& position(glm::vec3 position);
        TransformBuilder& size(glm::vec3 size);

        TransformComp build() {
            return this->transform;
        }

        operator TransformComp() {
            return this->transform;
        }
    private:
        TransformComp transform;
    };
}
