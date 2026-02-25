#pragma once
#include <espch.h>
#include "mesh.h"
#include "shader.h"
#include "texture.h"

namespace engine {

#define MODEL_TEXTURED    0b0001
#define MODEL_IMMUNE      0b0010

    struct Material {
        Shader shader;
        Texture texture;
        glm::vec2 tex_repeat;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
        bool is_textured;

        void print();
    };

    struct MaterialBuilder {
        MaterialBuilder();

        MaterialBuilder& set_color(glm::vec3 color);
        MaterialBuilder& set_ambient(glm::vec3 ambient);
        MaterialBuilder& set_diffuse(glm::vec3 diffuse);
        MaterialBuilder& set_specular(glm::vec3 specular);
        MaterialBuilder& set_shininess(float shininess);

        MaterialBuilder& set_shader(Shader shader);
        MaterialBuilder& set_texture(Texture texture);
        MaterialBuilder& set_tex_repeat(glm::vec2 repeat);
        Material build();

        operator Material() {
            return material;
        }

    private:
        Material material;
    };


    struct ModelComp {
        Material material;
        Mesh mesh;
        size_t layer;
        bool is_immune;

        ModelComp();

        ModelComp(Mesh mesh, Material material, size_t layer = 0, bool is_immune = false);
    };
}
