#include "entity.h"
#include "components/hierarchy.h"
#include "components/physicbody.h"
#include "components/tag.h"

namespace engine {
    Entity::Entity(Scene* scene, entt::entity id) 
        :  entity_id(id), scene(scene)
    {

    }


    bool Entity::is_child() {
        return has_component<ParentComp>();
    }

    bool Entity::is_parent() {
        return has_component<ChildrenComp>();
    }

    Entity& Entity::get_parent() {
        DU_ASSERT(!is_child(), "Entity {} does not have a parent", uuid());
        return get_component<ParentComp>().parent;
    }

    std::vector<UUID>& Entity::get_children() {
        DU_ASSERT(!is_parent(), "Entity {} does not have children", uuid());
        return get_component<ChildrenComp>().children;
    }

    void Entity::make_child_of(UUID parent) {
        DU_ASSERT(parent == uuid(), "Trying to set {} a parent of itself");
        if(is_child()) {
            Entity& tmp = get_parent();
            DU_ASSERT(tmp.uuid() == parent, "Trying to make {} a child of {} twice", uuid(), parent);
            tmp.remove_child(uuid());
            tmp = Entity::null();
        }
        Entity tmp = scene->uuid_to_entity(parent);
        tmp.add_child(uuid());
    }

    void Entity::add_child(UUID child) {
        if (!is_parent()) {
            DU_ASSERT(child == uuid(), "Trying to make {} a parent of itself", child);
            add_component<ChildrenComp>().children.push_back(child);
            Entity tmp_child = scene->uuid_to_entity(child);
            if (tmp_child.has_component<ParentComp>()) {
                DU_ASSERT(tmp_child.get_parent(), "Entity {} already has a parent {}", tmp_child.uuid(), tmp_child.get_parent().uuid());
                tmp_child.get_parent() = *this;
            }
            tmp_child.add_component<ParentComp>(*this);
            return;
        }

        auto& children = get_component<ChildrenComp>().children;
        DU_ASSERT(std::find(children.begin(), children.end(), child) != children.end(), "add_child(): Entity {} is already a child of {}", child, uuid());
        children.push_back(child);

        Entity tmp_child = scene->uuid_to_entity(child);
        if (tmp_child.has_component<ParentComp>()) {
            DU_ASSERT(tmp_child.get_parent(), "Entity {} already has a parent {}", tmp_child.uuid(), tmp_child.get_parent().uuid());
            tmp_child.get_parent() = *this;
        }
        tmp_child.add_component<ParentComp>(*this);

        if (!has_component<PhysicsBodyComp>()) return;

        make_physically_dominant(*this);
    }

    void Entity::add_children(const std::vector<UUID>& children) {
        for (const auto& child : children) {
            add_child(child);
        }
    }

    void Entity::remove_children() {
        const auto& children = get_children();
        //physically_disown_children(*this);
        for (const auto& child : children) {
            remove_child(child);
        }
        remove_component<ChildrenComp>();
    }

    void Entity::remove_child(UUID child) {
        auto& children = get_children();

        DU_ASSERT(std::find(children.begin(), children.end(), child) == children.end(), "remove_child(): Entity {} is not a child of {}", child, uuid());
        Entity child_e = scene->uuid_to_entity(child);

        if (child_e.has_component<PhysicsBodyComp>())
            make_physically_dominant(child_e);

        child_e.remove_parent();
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
    }

    void Entity::remove_parent() {
        DU_ASSERT(!is_child(), "Entity {} does not have a parent to remove", uuid());
        remove_component<ParentComp>();
    }



    bool is_ancestor_of(Scene& scene, UUID ancestor, UUID subject) {
        Entity sub = scene.uuid_to_entity(subject);
        while (sub.has_component<ParentComp>()) {
            ParentComp parent = sub.get_component<ParentComp>();
            if (parent.parent.uuid() == ancestor) return true;
            sub = parent.parent;
        }
        return false;
    }
}
