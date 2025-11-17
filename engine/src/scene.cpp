#include "scene.h"
#include "entity.h"
#include "manager.h"

namespace engine {

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
        DU_ASSERT(uuids.find(uuid) == uuids.end(), "ERROR: Unknown UUID {}", uuid.get_uuid());
        return uuids[uuid];
    }

    UUID Scene::entt_to_uuid(entt::entity id) {
        DU_ASSERT(!registry.valid(id), "Trying to convert non existant id to uuid");
        for (auto& [uuid, entt] : uuids) {
            if (entt == id) {
                return uuid;
            }
        }

        DU_ASSERT(true, "Could not convert id to uuid");
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

    Shader Scene::get_shader(const char* name) {
        DU_ASSERT(manager->shader_lib.find(name) == manager->shader_lib.end(), "Could not find registered shader [{}]", name);

        return manager->shader_lib.at(std::string(name));
    }
    Texture Scene::get_texture(const char* name) {
        DU_ASSERT(manager->texture_lib.find(name) == manager->texture_lib.end(), "Could not find registered texture [{}]", name);

        return manager->texture_lib.at(std::string(name));
    }
    Model Scene::get_model(const char* name) {
        DU_ASSERT(manager->model_lib.find(name) == manager->model_lib.end(), "Could not find registered model [{}]", name);

        return manager->model_lib.at(std::string(name));
    }

    } // namespace engine
