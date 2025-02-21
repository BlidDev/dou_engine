#pragma once
#include "model.h"
#include "scene.h"

namespace engine {

    class SceneManager {
    public:
        SceneManager();
        Scene* register_scene(const char* name, Scene* scene);
        Scene* get_scene(const char* name);
        void set_current(const char* name);
        void end_scene(Scene* scene);
        void end();

        void write_scene_to_file(const char* path, Scene* scene);
        Scene* scene_from_file(const char* path);

        void register_model(const char* name, VAOType format, float vertices[], unsigned int size);

        Scene* get_current();
        ~SceneManager();

    public:
        std::string current;
        bool switched;
        Window main_window;
    private:
        std::unordered_map<std::string, Scene*>scenes;
        std::unordered_map<std::string, Model>models;
    };
}
