#pragma once
#include "components/uuid.h"
#include "egassert.h"
#include <espch.h>
namespace engine {

    class SceneManager;
    class Entity;
    class Scene {
    public:
        Scene(const std::string name) : name(name) {}
        virtual void on_create() {};
        virtual void on_update(float dt)  {};
        virtual void on_end() {};
        
        virtual bool should_close() { return false; };
        virtual ~Scene() {}

        Entity create_entity();
        Entity create_entity_with_uuid(uint64_t uuid);
        Entity uuid_to_entity(UUID uuid);
        entt::entity uuid_to_entt(UUID uuid);

        void add_from_file(const char* path);

        template <typename T>
        T& get_uuid_component(UUID uuid) {
            EG_ASSERT(uuids.find(uuid) == uuids.end(), "Unknown UUID {}", uuid.get_uuid());
            entt::entity entity = uuids[uuid];
            EG_ASSERT(!registry.any_of<T>(entity), "Trying to get non existant uuid component {} from entity {}", typeid(T).name(), uuid.get_uuid());
            return registry.get<T>(entity);
        }
        
    public:
        entt::registry registry;
        SceneManager* manager;
        UUID main_camera = 0;
        std::string name;
        std::unordered_map<UUID, entt::entity> uuids;
    };


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

        Scene* get_current();
        ~SceneManager();

    public:
        std::string current;
        bool switched;
    private:
        std::unordered_map<std::string, Scene*>scenes;
    };
}
