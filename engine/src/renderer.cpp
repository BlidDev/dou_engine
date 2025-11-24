#include "renderer.h"
#include "manager.h"
#include "texture.h"

#include "components/light.h"

namespace engine {

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
        is_framebuffer = false;
        framebuffer = 0;
        framebuffer_texture = 0;
    }


    RenderData::RenderData() {
        ubos = {};
        counter = 0;
        bounded = 0;
        ambient = {1.0f, 1.0f, 1.0f};
        ambient_strength = 0.1f;
        max_lights = 32;
        clear_color = {0.0f, 0.0f, 0.0f, 1.0f};
        clear_flags = GL_COLOR_BUFFER_BIT;
    }

    RenderData& RenderData::add(const char* name, size_t size) {
        DU_ASSERT(does_ubo_exist(name, ubos), "UBO {} already exists", name);

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


    void set_clear_color(RenderData& data, glm::vec4 color) {
        data.clear_color = color;
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

    void set_layer_to_framebuffer(SceneManager* manager, size_t layer) {
        DU_ASSERT(layer >= MAX_RENDER_LAYERS || layer < 0, "Trying to set invalid layer [{}] to a framebuffer layer");
        LayerAtrb& atrb = manager->render_data.layers_atrb[layer];

        glGenFramebuffers(1, &atrb.framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, atrb.framebuffer);
    
        
        unsigned int texture_color_buffer;
        glGenTextures(1, &texture_color_buffer);
        glBindTexture(GL_TEXTURE_2D, texture_color_buffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, manager->render_data.screen_w, manager->render_data.screen_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color_buffer, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, manager->render_data.screen_w, manager->render_data.screen_h); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            DU_CORE_ERROR("Could not make frame buffer for layer {}", layer);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        Texture final;

        std::string name = std::format("FRAMEBUFFER{}",layer);
        final.texture = texture_color_buffer,
        final.w = manager->render_data.screen_w,
        final.h = manager->render_data.screen_h,
        final.nrc = -1,
        final.path = name;

        DU_CORE_INFO("Created new framebuffer texture [{}]", name);

        atrb.is_framebuffer = true;
        atrb.framebuffer_texture =  final;
        manager->texture_lib.insert(std::make_pair(name, final));
        
    }


    void make_default_ubos(SceneManager* manager) {
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
