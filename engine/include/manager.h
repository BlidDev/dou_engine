#pragma once
#include "model.h"
#include "renderer.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

namespace engine {

    class SceneManager {
    public:
        SceneManager();
        Scene* register_scene(const char* name, Scene* scene);
        Scene* get_scene(const char* name);
        void set_current(const char* name);
        void clear_scene(Scene* scene);
        void end_scene(Scene* scene);
        void end();

        void write_scene_to_file(const char* path, Scene* scene);

        void register_shader(const char* path);
        void register_texture(const char* path);
        void register_texture(std::string name, Texture texture);
        void register_model(const char* name, Model model);

        LayerAtrb* get_layer_atrb(size_t layer);

        Scene* get_current();
        ~SceneManager();

    public:
        std::string current;
        bool switched;
        Window main_window;
        std::unordered_map<std::string, Shader>shader_lib;
        std::unordered_map<std::string, Texture>texture_lib;
        std::unordered_map<std::string, Model>model_lib;
        RenderData render_data;

        std::filesystem::path root_path;
    private:
        std::unordered_map<std::string, Scene*>scenes;
    };

    void update_render_data(SceneManager* manager, Scene* current);
}

