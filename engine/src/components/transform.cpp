#include "components/transform.h"

namespace engine {
    TransformBuilder& TransformBuilder::position(Vector3 position) {
        transform.position = position;
        return *this;
    }
    TransformBuilder& TransformBuilder::size(Vector3 size) {
        transform.size = size;
        return *this;
    }

}
