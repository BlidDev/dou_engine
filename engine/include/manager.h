#pragma once
#include "mesh.h"
#include "project.h"
#include "renderer.h"
#include "scene.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include "project.h"

namespace engine {

    using SceneLib = std::unordered_map<std::string, std::unique_ptr<Scene>>;
    class SceneManager {
    public:
        SceneManager();
        Scene* register_scene(const char* name, std::unique_ptr<Scene> scene);
        Scene* register_scene(const char* name, std::unique_ptr<Scene>& scene);

        /**
         * register_scene_raw - overrides the unique_ptr gaurd and pass a raw pointer instead
         * @name: name of the scene
         * @scene: raw pointer to a scene object
         *
         * Note: This function does take control over the raw pointer meaning it will
         * automatically delete it when Scene Manager goes out of scope!
         */

        Scene* register_scene_raw(const char* name, Scene* scene);

        template <typename T, typename ...Args>
        Scene* register_scene(const char* name, Args&&... args) {
            return register_scene(name, std::make_unique<T>(std::forward<Args>(args)...));
        }


        Scene* get_scene(const char* name);
        void set_current(const char* name);
        void clear_scene(Scene* scene);
        void end_scene(Scene* scene);
        void end();

        void write_scene_to_file(const char* path, Scene* scene);

        void register_shader(const char* path);
        void register_texture(const char* path);
        void register_texture(std::string name, Texture texture);
        void register_mesh(const char* name, Mesh mesh);
        void register_script(const char* path);

        LayerAtrb* get_layer_atrb(size_t layer);

        const std::filesystem::path& root_path();

        size_t num_of_scenes();
        const SceneLib& get_scenes() const;

        Scene* get_current();
        ~SceneManager();

    public:
        std::string current;
        bool switched;
        Scene* old;
        
        Window main_window;
        std::unordered_map<std::string, Shader>     shader_lib;
        std::unordered_map<std::string, Texture>    texture_lib;
        std::unordered_map<std::string, Mesh>       mesh_lib;
        std::unordered_map<std::string, std::string>script_lib;
        RenderData render_data;

        ProjectData project_data;
    private:
        SceneLib scenes;
    };

    std::string extract_scene_name(const char* path);
}

