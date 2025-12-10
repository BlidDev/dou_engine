#include <espch.h>
#include "entity.h"

namespace engine {

    struct ParentComp {
        ParentComp() {
            parent = {};
        }
        ParentComp(Entity parent) : parent(parent) {}

        operator entt::entity() {
            return parent.id();
        }

        Entity parent;
    };


    struct ChildrenComp {
        ChildrenComp() {
            children = {};
        }

        ChildrenComp(Entity self, Scene* scene, std::vector<UUID> children = {}) {
            if (!scene || children.empty()) 
                return;

            this->children.reserve(children.size());
            for (const auto& child : children) {
                Entity tmp = scene->uuid_to_entity(child);
                DU_ASSERT(tmp.has_component<ParentComp>(), "Entity {} already has a parent", child);
                DU_ASSERT(tmp == self, "Trying to make {} a parent of itself", self.uuid());
                tmp.add_component<ParentComp>(self);
                this->children.push_back(child);
            }
        }

        std::vector<UUID> children;
    };

}
