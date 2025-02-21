#include "scene.h"
#include "entity.h"

namespace engine 
{

    Entity Scene::create_entity() {
        return create_entity_with_uuid(UUID());
    }

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
        EG_ASSERT(uuids.find(uuid) == uuids.end(), "ERROR: Unknown UUID {}", uuid.get_uuid());
        return uuids[uuid];
    }

}
