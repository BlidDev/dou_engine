#pragma once
#include "shader.h"
#include <espch.h>

namespace engine {

    struct UBO {
        unsigned int handler;
        unsigned int binding_point;
        size_t size;
        std::string name;

        UBO& set_sub(size_t start, size_t size, void* value);
    };

    class RenderData {
    public:
        RenderData();
        RenderData& add(const char* name, size_t size);

        UBO& get(const char* name);

        RenderData& bind(const char* name);
        RenderData& sub(size_t start, size_t size, void* value);
        void unbind();

        std::vector<UBO> ubos;
    private:
        unsigned int counter;
        unsigned int bounded;
    public:
        glm::vec3 ambient;
        float ambient_strength;
        unsigned int max_lights;
    };

    void ubos_shaders_bind(RenderData& data, std::unordered_map<std::string, Shader>& shaders);
}
