#pragma once
#include "components/uuid.h"
#include "model.h"
#include <espch.h>
#include "shader.h"
#include "texture.h"
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
        UUID entt_to_uuid(entt::entity id);
        void remove_entity(UUID uuid);

        void add_from_file(const char* path, std::filesystem::path root = "");

        template <typename T>
        T& get_uuid_component(UUID uuid) {
            DU_ASSERT(uuids.find(uuid) == uuids.end(), "Unknown UUID {}", uuid.get_uuid());
            entt::entity entity = uuids[uuid];
            DU_ASSERT(!registry.any_of<T>(entity), "Trying to get non existant uuid component {} from entity {}", typeid(T).name(), uuid.get_uuid());
            return registry.get<T>(entity);
        }

        void register_shader(const char* path);

        Shader get_shader(const char* name);
        Texture get_texture(const char* name);
        Model get_model(const char* name);
        
    public:
        entt::registry registry;
        SceneManager* manager;
        std::string name;
        std::unordered_map<UUID, entt::entity> uuids;

    public:
        UUID main_camera = 0;
        glm::vec3 ambient = {1.0f, 1.0f, 1.0f};
        float ambient_strength = 0.1f;
    };
}
