#include "components/transform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace engine {

    void set_size(glm::mat4& model, glm::vec3& size) {
        model[0][0] = size.x;
        model[1][1] = size.y;
        model[2][2] = size.z;
    }

    void set_rotation(glm::mat4& model, glm::vec3& angles) {
        model = glm::rotate(model, glm::radians(angles.x), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(angles.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(angles.z), {0.0f, 0.0f, 1.0f});
    }
    
    glm::mat4 TransformComp::get_model() {
        model = glm::mat4(1.0f);
        //set_size(model, size);
        model = glm::scale(model, size);
        set_rotation(model, rotation);
        model = glm::translate(model, position);
        return model;
    }


    TransformBuilder& TransformBuilder::position(glm::vec3 position) {
        transform.position = position;
        return *this;
    }
    TransformBuilder& TransformBuilder::size(glm::vec3 size) {
        transform.size = size;
        return *this;
    }

    TransformBuilder& TransformBuilder::rotation(glm::vec3 rotation) {
        transform.rotation = rotation;
        return *this;
    }

}
