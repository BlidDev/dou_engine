#pragma once
#include "components/uuid.h"
#include "mesh.h"
#include <espch.h>
#include "renderer.h"
#include "shader.h"
#include "texture.h"
namespace engine {

    class SceneManager;
    class Entity;

    template<size_t size>
    using DeleteCache = std::array<UUID, size>;

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
        void remove_entity(UUID uuid, bool delete_children = true);

        static constexpr size_t MAX_DELETE_CACHE = 128;
        template <size_t size>
        void cache_remove_entity(UUID uuid,
                                 DeleteCache<size>& cache,
                                 size_t& index,
                                 bool delete_children = true);

        void add_from_file(const std::filesystem::path &path, const std::filesystem::path &root = "");

        template <typename T>
        T& get_uuid_component(UUID uuid) {
            const auto& it = uuids.find(uuid);
            DU_ASSERT(it == uuids.end(), "Unknown UUID {}", uuid.get_uuid());
            entt::entity entity = it->second;
            DU_ASSERT(!registry.any_of<T>(entity), "Trying to get non existant uuid component {} from entity {}", typeid(T).name(), uuid.get_uuid());
            return registry.get<T>(entity);
        }

        template <typename T>
        bool has_uuid_component(UUID uuid) {
            const auto& it = uuids.find(uuid);
            DU_ASSERT(it == uuids.end(), "Unknown UUID {}", uuid.get_uuid());
            entt::entity entity = it->second;
            return registry.any_of<T>(entity);
        }

        void register_shader(const std::filesystem::path& path);

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

    // ==========================================================

    template <size_t size>
    static void remove_entity_helper(Scene* scene, UUID uuid, DeleteCache<size> &cache, size_t &index, bool delete_children, bool first = true) {
        Entity e = scene->uuid_to_entity(uuid);
        DU_ASSERT(index >= size - 1, "Delete cache overflow ({}) for scene {}", size, scene->name);
        if (e.is_child() && first) {
            e.get_parent().remove_child(uuid);
        }

        if (e.is_parent()) {
            if (delete_children) {
                for (const auto &child : e.get_children())
                    remove_entity_helper(scene, child, cache, index, true, false);
            }
            else {
                e.remove_children();
            }
        }

        scene->uuids.erase(uuid);
        scene->registry.destroy(e.id());
        cache[index++] = uuid;
    }

    template <size_t size>
    void Scene::cache_remove_entity(UUID uuid,
                                    DeleteCache<size> &cache,
                                    size_t &index,
                                    bool delete_children)
    {
        remove_entity_helper(this, uuid, cache, index, delete_children);
    }

}
