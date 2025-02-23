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

}
