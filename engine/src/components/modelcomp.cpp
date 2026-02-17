#include "components/modelcomp.h"
#include "renderer.h"


namespace engine {
    MaterialBuilder::MaterialBuilder() {
        material = Material {
            .shader = {},
            .texture = Texture(),
            .ambient = {0.0f, 0.0f, 0.0f},
            .diffuse = {0.0f, 0.0f, 0.0f},
            .specular = {0.0f, 0.0f, 0.0f},
            .attributes = 0b0000
        };
    }

    void Material::print() {
        DU_CORE_INFO("Material: {} {} {}",
                shader.path, shader.program, attributes);
    }

    MaterialBuilder& MaterialBuilder::set_color(glm::vec3 color) {
        material.ambient = color; 
        material.diffuse = color; 
        material.specular = {1.0f, 1.0f, 1.0f}; 
        material.shininess = 32.0f;
        material.attributes |= MODEL_FILLED;
        return *this;
    }

    MaterialBuilder& MaterialBuilder::set_ambient(glm::vec3 ambient) {
        material.ambient = ambient;
        return *this;
    }

    MaterialBuilder& MaterialBuilder::set_diffuse(glm::vec3 diffuse) {
        material.diffuse = diffuse;
        return *this;
    }

    MaterialBuilder& MaterialBuilder::set_specular(glm::vec3 specular) {
        material.specular = specular;
        return *this;
    }

    MaterialBuilder& MaterialBuilder::set_shininess(float shininess) {
        material.shininess = shininess;
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
        mesh = {0,0,0,0,0,"UNKNOWN"};
        layer = 0;
    }

        ModelComp::ModelComp(Mesh mesh, Material material, size_t layer) {
            this->mesh = mesh;
            this->material = material;
            this->layer = layer;
            DU_ASSERT(layer >= MAX_RENDER_LAYERS || layer < 0, "Invalid layer number [{}]", layer);
        }
    


}
