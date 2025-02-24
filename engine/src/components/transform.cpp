#include "components/transform.h"

namespace engine {
    TransformBuilder& TransformBuilder::position(glm::vec3 position) {
        transform.position = position;
        return *this;
    }

    TransformBuilder& TransformBuilder::size(glm::vec3 size) {
        transform.size = size;
        return *this;
    }

    glm::mat4 transform_to_m4(TransformComp& transform) {
        // TODO
        return glm::mat4(0.0f);
    }
}
