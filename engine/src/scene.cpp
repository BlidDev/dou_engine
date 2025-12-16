#include "scene.h"
#include "entity.h"
#include "manager.h"

namespace engine {

    Scene::Scene(std::string name) {
        this->name = name;
        manager = nullptr;
        registry = {};
        uuids = {};
        main_camera = 0;
        file_path = "";
        s_render_data = {};
    }

    Entity Scene::create_entity() { return create_entity_with_uuid(UUID()); }

    Entity Scene::create_entity_with_uuid(uint64_t uuid) {
        Entity tmp(this, registry.create());
        tmp.add_component<UUID>(uuid);
        uuids.insert(std::make_pair(uuid, tmp.id()));
        return tmp;
    }

    Entity Scene::uuid_to_entity(UUID uuid) {
        entt::entity entity = uuid_to_entt(uuid);
        return {this, entity};
    }

    entt::entity Scene::uuid_to_entt(UUID uuid) {
        const auto& it = uuids.find(uuid);
        DU_ASSERT(it == uuids.end(), "ERROR: Unknown UUID {}", uuid.get_uuid());
        return it->second;
    }

    UUID Scene::entt_to_uuid(entt::entity id) {
        DU_ASSERT(!registry.valid(id), "Trying to convert non existant id to uuid");
        const auto it = std::find_if(uuids.begin(), uuids.end(), [&id](const auto& pair){return pair.second == id;});
        DU_ASSERT(it == uuids.end(), "Could not convert id {} to uuid", (uint32_t)id);
        return it->first;
    }

    void Scene::remove_entity(UUID uuid) {
        Entity e = uuid_to_entity(uuid);
        if (e.is_child()) {
            e.get_parent().remove_child(uuid);
        }

        uuids.erase(uuid);
        registry.destroy(e.id());
    }

    void Scene::register_shader(const char* name) {
        manager->register_shader(name);
    }

    Shader& Scene::get_shader(const char* name) {
        const auto& it = manager->shader_lib.find(name);
        DU_ASSERT(it == manager->shader_lib.end(), "Could not find registered shader [{}]", name);

        return it->second;
    }
    Texture& Scene::get_texture(const char* name) {
        const auto& it = manager->texture_lib.find(name);
        DU_ASSERT(it == manager->texture_lib.end(), "Could not find registered texture [{}]", name);

        return it->second;
    }
    Mesh& Scene::get_mesh(const char* name) {
        const auto& it = manager->mesh_lib.find(name);
        DU_ASSERT(it == manager->mesh_lib.end(), "Could not find registered model [{}]", name);

        return it->second;
    }

    std::string& Scene::get_script(const char* name) {
        const auto& it = manager->script_lib.find(name);
        DU_ASSERT(it == manager->script_lib.end(), "Could not find registered script [{}]", name);

        return it->second;
    }
} 
