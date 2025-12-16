#pragma once
#include <espch.h>
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "renderer.h"

namespace engine {

#define MODEL_FILLED      0b0001
#define MODEL_TEXTURED    0b0010
#define MODEL_IMMUNE      0b0100

    struct Material {
        Shader shader;
        Texture texture;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
        int attributes;

        void print();
    };

    struct MaterialBuilder {
        MaterialBuilder();

        MaterialBuilder& set_color(glm::vec3 color);
        MaterialBuilder& set_ambient(glm::vec3 ambient);
        MaterialBuilder& set_diffuse(glm::vec3 diffuse);
        MaterialBuilder& set_specular(glm::vec3 specular);
        MaterialBuilder& set_shiniess(float shininess);

        MaterialBuilder& set_attributes(int attributes);
        MaterialBuilder& set_shader(Shader shader);
        MaterialBuilder& set_texture(Texture texture);
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

        ModelComp();

        ModelComp(Mesh mesh, Material material, size_t layer = 0);
        
    };
}
