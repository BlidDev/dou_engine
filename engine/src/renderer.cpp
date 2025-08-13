#include "renderer.h"


namespace engine {

    bool does_ubo_exist(const char* name, std::vector<UBO>& ubos) {
        for (auto& ubo : ubos) {
            if (name == ubo.name) {
                return true;
            }
        }
        return false;
    }

    void ubos_shaders_bind(RenderData& data, std::unordered_map<std::string, Shader>& shaders) {
        for (auto& ubo : data.ubos) {
            for (auto& [_, shader] : shaders) {
                unsigned int index = glGetUniformBlockIndex(shader.program, ubo.name.c_str());
                glUniformBlockBinding(shader.program, index, ubo.binding_point);
            }
        }
    }

    RenderData::RenderData() {
        ubos = {};
        counter = 0;
        bounded = 0;
        ambient = {1.0f, 1.0f, 1.0f};
        ambient_strength = 0.1f;
        max_lights = 32;
    }

    RenderData& RenderData::add(const char* name, size_t size) {
        EG_ASSERT(does_ubo_exist(name, ubos), "UBO {} already exists", name);

        unsigned int handler;
        glGenBuffers(1, &handler);
        glBindBuffer(GL_UNIFORM_BUFFER, handler);
        glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, counter, handler, 0, size);

        ubos.push_back({handler, counter, size, name});
        counter++;
        return *this;
    }

    UBO& RenderData::get(const char* name) {
        for (UBO& ubo : ubos) {
            if (name == ubo.name) 
                return ubo;
        }
        EG_ASSERT(true, "No UBO named {}", name);
    }

    RenderData& RenderData::bind(const char* name) { 
        UBO& ubo = get(name);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo.handler);
        bounded = ubo.handler;
        return *this;
    }

    RenderData& RenderData::sub(size_t start, size_t size, void* value) {
        glBufferSubData(GL_UNIFORM_BUFFER, start, size, value);
        return *this;
    }

    void RenderData::unbind() { 
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

}
