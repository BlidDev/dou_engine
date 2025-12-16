#pragma once
#include "components/uuid.h"
#include "model.h"
#include <espch.h>
#include "renderer.h"
#include "shader.h"
#include "texture.h"
namespace engine {

    class SceneManager;
    class Entity;
    class Scene {
    public:
        Scene(const std::string name);

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
            const auto& it = uuids.find(uuid);
            DU_ASSERT(it == uuids.end(), "Unknown UUID {}", uuid.get_uuid());
            entt::entity entity = it->second;
            DU_ASSERT(!registry.any_of<T>(entity), "Trying to get non existant uuid component {} from entity {}", typeid(T).name(), uuid.get_uuid());
            return registry.get<T>(entity);
        }

        void register_shader(const char* path);

        Shader& get_shader(const char* name);
        Texture& get_texture(const char* name);
        Mesh& get_mesh(const char* name);
        std::string& get_script(const char* name);
        
    public:
        SceneRenderData s_render_data;
        entt::registry registry;
        SceneManager* manager;
        std::string name;
        std::unordered_map<UUID, entt::entity> uuids;

    public:
        UUID main_camera = 0;
    public:
        std::filesystem::path file_path;
    };
}
