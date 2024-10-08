#pragma once
#include <entt.hpp>

namespace engine {

    class SceneManager;
    class Entity;
    class Scene {
    public:
        Scene() {}
        virtual void on_create() = 0;
        virtual void on_update(float dt)  {};
        virtual void on_end() {};
        
        virtual bool should_close() { return false; };
        virtual ~Scene() {}

        Entity create_entity();
    public:
        entt::registry registry;
        SceneManager* manager;
    };


    class SceneManager {
    public:
        SceneManager();
        void register_scene(const char* name, Scene* scene);
        Scene* get_scene(const char* name);
        void set_current(const char* name);
        void end_scene(Scene* scene);
        void end();
        Scene* get_current();
        ~SceneManager();

    public:
        std::string current;
        bool switched;
    private:
        std::unordered_map<std::string, Scene*>scenes;
    };
}
