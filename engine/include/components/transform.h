#pragma once
#include <espch.h>
#include <glm/glm.hpp>


namespace engine {

    struct TransformComp {
        glm::vec3 position;
        glm::vec3 size;
        glm::vec3 rotation;

        TransformComp(glm::vec3 position = {0.0f, 0.0f, 0.0f},
                      glm::vec3 size = {1.0f, 1.0f, 1.0f}, 
                      glm::vec3 rotation = {0.0f, 0.0f, 0.0f}) {
            this->position = position;
            this->size = size;
            this->rotation = rotation;
            model = glm::mat4(1.0f);
        }

        glm::mat4 get_model();

    private:
        glm::mat4 model;
    };

    struct TransformBuilder {
        TransformBuilder& position(glm::vec3 position);
        TransformBuilder& size(glm::vec3 size);
        TransformBuilder& rotation(glm::vec3 rotation);

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
