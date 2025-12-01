#include "renderer.h"
#include "manager.h"
#include "texture.h"

#include "components/light.h"

namespace engine {

    UBO::UBO(uint32_t handler, uint32_t binding, size_t size, const char* name) {
        this->handler = handler;
        this->binding_point = binding;
        this->size = size;
        this->name = name;
    }

    UBO::UBO(const UBO& other) {
        handler = other.handler;
        binding_point = other.binding_point;
        size = other.size;
        name = other.name;

    }

    UBO& UBO::operator=(const UBO& other) {
        handler = other.handler;
        binding_point = other.binding_point;
        size = other.size;
        name = other.name;
        return *this;
    }

    UBO::UBO(UBO&& other) {
        handler = other.handler;
        binding_point = other.binding_point;
        size = other.size;
        name = std::move(other.name);
    }

    UBO& UBO::operator=(UBO&& other) {
        handler = other.handler;
        binding_point = other.binding_point;
        size = other.size;
        name = std::move(other.name);
        return *this;
    }

    bool does_ubo_exist(const char* name, std::vector<UBO>& ubos) {
        return std::find_if(ubos.begin(), ubos.end(), [&name](const UBO& u) {return u.name == name;}) != ubos.end();
    }

    void ubos_shaders_bind(RenderData& data, std::unordered_map<std::string, Shader>& shaders) {
        for (const auto& ubo : data.ubos) {
            for (const auto& [_, shader] : shaders) {
                unsigned int index = glGetUniformBlockIndex(shader.program, ubo.name.c_str());
                glUniformBlockBinding(shader.program, index, ubo.binding_point);
            }
        }
    }




    LayerAtrb::LayerAtrb() {
        depth = false;
        wireframe = false;
    }


    RenderData::RenderData() {
        ubos = {};
        counter = 0;
        bounded = 0;
        max_lights = 32;
        clear_flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
        layers_atrb = {};
    }

    RenderData& RenderData::add(const char* name, size_t size) {
        DU_ASSERT(does_ubo_exist(name, ubos), "UBO {} already exists", name);

        unsigned int handler;
        glGenBuffers(1, &handler);
        glBindBuffer(GL_UNIFORM_BUFFER, handler);
        glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, counter, handler, 0, size);

        ubos.emplace_back(handler, counter, size, name);
        counter++;
        return *this;
    }

    UBO& RenderData::get(const char* name) {
        for (UBO& ubo : ubos) {
            if (name == ubo.name) 
                return ubo;
        }
        DU_ASSERT(true, "No UBO named {}", name);
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

    void set_layer_depth(RenderData& data, size_t layer, bool flag) {
        DU_ASSERT(layer >= MAX_RENDER_LAYERS || layer < 0, "Trying to set depth flag to invalid layer [{}]");
        data.layers_atrb[layer].depth = flag;
    }

    void set_layer_wireframe(RenderData& data, size_t layer, bool flag) {
        DU_ASSERT(layer >= MAX_RENDER_LAYERS || layer < 0, "Trying to set wireframe flag to invalid layer [{}]");
        data.layers_atrb[layer].wireframe = flag;
    }

    void set_clear_flags(RenderData& data, int flags) {
        data.clear_flags = flags;
    }

    void make_default_ubos(SceneManager* manager) {
        manager->render_data.ubos.reserve(5);
        manager->render_data.add("Matrices", 2 * sizeof(glm::mat4));
        manager->render_data.add("Lighting", 2 * sizeof(glm::vec4));

        size_t max = 32;
        size_t dir_size = sizeof(engine::DirLightComp); 
        size_t pnt_size = 3 * sizeof(glm::vec4); 
        size_t spt_size = sizeof(SptLightComp) + sizeof(glm::vec4);

        manager->render_data.add("SptLights", spt_size * max + sizeof(int));
        manager->render_data.add("DirLights", dir_size * max + sizeof(int));
        manager->render_data.add("PntLights", pnt_size * max + sizeof(int));

        engine::ubos_shaders_bind(manager->render_data, manager->shader_lib);
    }
}
