#pragma once
#include <entt.hpp>
#include <string.h>

namespace engine {

    class SceneManager;
    class Entity;
    class Scene {
    public:
        Scene(const std::string name) : name(name) {}
        virtual void on_create();
        virtual void on_update(float dt)  {};
        virtual void on_end() {};
        
        virtual bool should_close() { return false; };
        virtual ~Scene() {}

        Entity create_entity();
    public:
        entt::registry registry;
        SceneManager* manager;
        const std::string name;
    };


    class SceneManager {
    public:
        SceneManager();
        void register_scene(const char* name, Scene* scene);
        Scene* get_scene(const char* name);
        void set_current(const char* name);
        void end_scene(Scene* scene);
        void end();

        void write_scene_to_file(const char* path, Scene* scene);
        Scene* scene_from_file(const char* path);

        Scene* get_current();
        ~SceneManager();

    public:
        std::string current;
        bool switched;
    private:
        std::unordered_map<std::string, Scene*>scenes;
    };
}
