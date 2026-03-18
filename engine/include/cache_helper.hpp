#pragma once
#include "scene.h"
#include "entity.h"


namespace engine {

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
                DeleteCache<size>& cache,
                size_t &index,
                bool delete_children)
        {
            remove_entity_helper(this, uuid, cache, index, delete_children);
        }
}
