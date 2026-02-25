#include "components/modelcomp.h"
#include "renderer.h"


namespace engine {
    MaterialBuilder::MaterialBuilder() {
        material = Material {
            .shader = {},
            .texture = Texture(),
            .tex_repeat = {1.0f, 1.0f},
            .ambient = {0.0f, 0.0f, 0.0f},
            .diffuse = {0.0f, 0.0f, 0.0f},
            .specular = {0.0f, 0.0f, 0.0f},
            .is_textured = false
        };
    }

    void Material::print() {
        DU_CORE_INFO("Material-> path:{} shader:{} is_textured:{}",
                shader.path, shader.program, is_textured);
    }

    MaterialBuilder& MaterialBuilder::set_color(glm::vec3 color) {
        material.ambient = color; 
        material.diffuse = color; 
        material.specular = {1.0f, 1.0f, 1.0f}; 
        material.shininess = 32.0f;
        material.is_textured = false;
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

    MaterialBuilder& MaterialBuilder::set_shader(Shader shader) {
        material.shader = shader;
        return *this;
    }

    MaterialBuilder& MaterialBuilder::set_texture(Texture texture) {
        material.texture = texture;
        material.is_textured = true;
        return *this;
    }

    MaterialBuilder& MaterialBuilder::set_tex_repeat(glm::vec2 repeat) {
        material.tex_repeat = repeat;
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

        ModelComp::ModelComp(Mesh mesh, Material material, size_t layer, bool is_immune) {
            this->mesh = mesh;
            this->material = material;
            this->layer = layer;
            this->is_immune = is_immune;
            DU_ASSERT(layer >= MAX_RENDER_LAYERS || layer < 0, "Invalid layer number [{}]", layer);
        }
    


}
