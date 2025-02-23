#pragma once
#include <glm/glm.hpp>
#include "model.h"
#include "shader.h"

namespace engine {

#define MODEL_FILLED    0b0001
#define MODEL_WIREFRAME 0b0010
#define MODEL_IMMUNE    0b0100

    struct Material {
        Shader shader;
        glm::vec4 color;
        int attributes;
    };

    struct MaterialBuilder {
        MaterialBuilder();

        MaterialBuilder& set_color(glm::vec4 color);
        MaterialBuilder& set_attributes(int attributes);
        MaterialBuilder& set_shader(Shader shader);
        Material build();

    private:
        Material material;
    };


    struct ModelComp {
        Material material;
        Model model;

        ModelComp(
                Model model,
                Material material
                ) {
            this->model = model;
            this->material = material;
        }
    };
}
