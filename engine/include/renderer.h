#pragma once
#include "shader.h"
#include <espch.h>

#define MAX_RENDER_LAYERS 16

namespace engine {

    struct UBO {
        uint32_t handler;
        uint32_t binding_point;
        size_t size;
        std::string name;

        UBO(uint32_t handler = 0, uint32_t binding = 0, size_t size = 0, const char* name = "UNSETUBO");
        UBO(const UBO& other);
        UBO& operator=(const UBO& other);

        UBO(UBO&& other);
        UBO& operator=(UBO&& other);

    };


    struct LayerAtrb {
        bool depth;
        bool wireframe;
        bool is_framebuffer;
        unsigned int framebuffer;
        unsigned int framebuffer_texture;
        LayerAtrb();
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
    public:
        size_t screen_w;
        size_t screen_h;
        LayerAtrb layers_atrb[MAX_RENDER_LAYERS];
        glm::vec4 clear_color;
        int clear_flags;
    };


    void ubos_shaders_bind(RenderData& data, std::unordered_map<std::string, Shader>& shaders);

    struct SceneManager;
    void set_clear_color(RenderData& data, glm::vec4 color);
    void set_layer_depth(RenderData& data, size_t layer, bool flag);
    void set_layer_wireframe(RenderData& data, size_t layer, bool flag);
    void set_clear_flags(RenderData& data, int flags);
    void set_layer_to_framebuffer(SceneManager* manager, size_t layer);


    void make_default_ubos(SceneManager* manager);
}

#include "formatting.h"

template<>
struct fmt::formatter<engine::UBO> : fmt::formatter<std::string>{
    auto format(const engine::UBO& u, format_context& ctx) const {
        return fmt::formatter<std::string>::format(fmt::format("UBO: {} H:{} | B:{} | S:{}", u.name, u.handler, u.binding_point, u.size),ctx);
    }
};

template<>
struct fmt::formatter<engine::RenderData> : fmt::formatter<std::string>{
    auto format(const engine::RenderData& d, format_context& ctx) const {
        return fmt::formatter<std::string>::format(
                fmt::format("\nUBOs: {}, sW: {}, sH: {}, mLights: {}\nAmbient: {} - Strength: {}\nClear: {}",
                    d.ubos.size(), d.screen_w, d.screen_h, d.max_lights, d.ambient, d.ambient_strength, d.clear_color), ctx);
    }
};

