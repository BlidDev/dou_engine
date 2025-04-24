#include "components/modelcomp.h"


namespace engine {
    MaterialBuilder::MaterialBuilder() {
        material = Material {
            .shader = {0, "UNKOWN"},
            .texture = Texture(),
            .color = {0.0f,0.0f,0.0f,0.0f},
            .attributes = 0b0000
        };
    }

    void Material::print() {
        EG_CORE_INFO("Material: {} {} ({}, {}, {}, {}) {}",
                shader.path, shader.program,
                color.r, color.g, color.b, color.a, attributes);
    }

    MaterialBuilder& MaterialBuilder::set_color(glm::vec4 color) {
        material.color = color;
        material.attributes |= MODEL_FILLED;
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

    MaterialBuilder& MaterialBuilder::set_texture(Texture texture) {
        material.texture = texture;
        material.attributes |= MODEL_TEXTURED;
        return *this;
    }
    Material MaterialBuilder::build() {
        return this->material;
    }

    ModelComp::ModelComp() {
        material = MaterialBuilder();
        model = {0,0,0,0,0,"UNKNOWN"};
    }
    


}
