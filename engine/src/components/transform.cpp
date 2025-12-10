#include "components/transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include "util.h"

namespace engine {
    void tmp_set_rotation(glm::mat4& model, glm::vec3& angles) {
        model = glm::rotate(model, glm::radians(angles.x), {1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(angles.y), {0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(angles.z), {0.0f, 0.0f, 1.0f});
    }


    glm::vec3  TransformComp::position() const { return v_position;}
    glm::vec3  TransformComp::size() const { return v_size; }
    glm::vec3  TransformComp::rotation() const { return v_rotation; }

    glm::vec3& TransformComp::position_ref() { rawed = true; return v_position; }
    glm::vec3& TransformComp::size_ref() { rawed = true; return v_size; }
    glm::vec3& TransformComp::rotation_ref() { rawed = true; return v_rotation; }

    void TransformComp::translate(const glm::vec3& v) {
        v_position += v;
        rawed = true;
    }
    void TransformComp::scale(const glm::vec3& v) {
        v_size += v;
        rawed = true;
    }
    void TransformComp::rotate(const glm::vec3& v) {
        v_rotation += v;
        rawed = true;
    }


    
    glm::mat4 TransformComp::get_model() {
        if (rawed)
            build_model();
        return model;
    }

    void TransformComp::set_model(const glm::mat4& value) {
        model = value;
        decompose();
        rawed = false;
    }

    void TransformComp::apply_mat(const glm::mat4& value) {
        model = value * model;
        decompose();
        rawed = false;
    }

    void TransformComp::build_model() {
        model = glm::translate(glm::mat4(1.0f), v_position);

        q_rotation = glm::quat(glm::radians(v_rotation));
        model = model * glm::mat4_cast(q_rotation);

        model = glm::scale(model, v_size);

        rawed = false;
    }

    void TransformComp::decompose() {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::quat rot;

        glm::decompose(model, v_size, rot, v_position, skew, perspective);

        q_rotation = rot;
        v_rotation = glm::degrees(glm::eulerAngles(rot));
    }

    TransformBuilder& TransformBuilder::position(glm::vec3 position) {
        transform.set_position(position);
        return *this;
    }
    TransformBuilder& TransformBuilder::size(glm::vec3 size) {
        transform.set_size(size);
        return *this;
    }

    TransformBuilder& TransformBuilder::rotation(glm::vec3 rotation) {
        transform.set_rotation(rotation);
        return *this;
    }

}
