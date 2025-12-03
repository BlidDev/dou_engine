#pragma once
#include <espch.h>


namespace engine {

    struct TransformComp {

        TransformComp(glm::vec3 position = {0.0f, 0.0f, 0.0f},
                      glm::vec3 size = {1.0f, 1.0f, 1.0f}, 
                      glm::vec3 rotation = {0.0f, 0.0f, 0.0f}) {
            this->v_position = position;
            this->v_size = size;
            this->v_rotation = rotation;
            model = glm::mat4(1.0f);
            rawed = true;
        }

        glm::vec3 position() const;
        glm::vec3 size() const;
        glm::vec3 rotation() const;

        glm::vec3& position_ref();
        glm::vec3& size_ref();
        glm::vec3& rotation_ref();

        void set_position(const glm::vec3& position) { v_position = position; rawed = true; }
        void set_size(const glm::vec3& size) { v_size = size; rawed = true; }
        void set_rotation(const glm::vec3& rotation) { v_rotation = rotation; rawed = true; }

        void translate(const glm::vec3& v);
        void scale(const glm::vec3& v);
        // 3 Euler angles to add to the current angle
        void rotate(const glm::vec3& v);

        glm::mat4  get_model();
        void       set_model(const glm::mat4& value);

        // Model = value * CurrentModel;
        void       apply_mat(const glm::mat4& value);


    private:
        void build_model();
        void decompose();

    private:
        glm::vec3 v_position;
        glm::vec3 v_size;
        glm::vec3 v_rotation;

        glm::mat4 model;
        glm::quat q_rotation;
        bool rawed;
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
