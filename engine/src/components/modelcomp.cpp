#include "components/modelcomp.h"


namespace engine {
    MaterialBuilder::MaterialBuilder() {
        material = Material {
            .shader = 0,
            .color = {0.5f,0.5f,0.5f,1.0f},
            .attributes = MODEL_FILLED
        };
    }

    MaterialBuilder& MaterialBuilder::set_color(glm::vec4 color) {
        material.color = color;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::set_attributes(int attributes) {
        material.attributes = attributes;
        return *this;
    }
    MaterialBuilder& MaterialBuilder::set_shader(Shader shader) {
        material.shader = shader;
        return *this;
    }
    Material MaterialBuilder::build() {
        return this->material;
    }


}
