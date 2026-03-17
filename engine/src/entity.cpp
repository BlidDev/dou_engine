#include "entity.h"
#include "component.h"

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

    static void remove_previous_parent(Entity& child, Entity& new_parent) {
        if (!child.is_child()) {
            child.add_component<ParentComp>(new_parent);
            return;
        }
        Entity& old_parent = child.get_parent();
        UUID new_parent_uuid = new_parent.uuid();

        DU_ASSERT(old_parent.uuid() == new_parent_uuid, "Trying to make {} a child of {} twice", child.uuid(), new_parent_uuid);
        DU_CORE_DEBUG_TRACE("Entity {} already has a parent {}, changing ownership", child.uuid(), old_parent.uuid());

        auto& op_children = old_parent.get_children();

        op_children.erase(std::remove(op_children.begin(), op_children.end(), child), op_children.end());

        old_parent = new_parent;
    }

    void Entity::make_child_of(UUID parent) {
        DU_ASSERT(parent == uuid(), "Trying to set {} a parent of itself");

        Entity tmp = scene->uuid_to_entity(parent);

        tmp.add_child(uuid());
    }

    void Entity::add_child(UUID child) {
        DU_ASSERT(child == uuid(), "Trying to make {} a parent of itself", child);
        if (!is_parent())
            add_component<ChildrenComp>();

        Entity tmp_child = scene->uuid_to_entity(child);
        remove_previous_parent(tmp_child, *this);

        auto& children = get_children();
        children.push_back(child);

        switch (get_tree_dominance(*this)) {
            case Dominance::Owned: make_physically_dominant(tmp_child); break;
            case Dominance::Dominant: make_physically_owned(tmp_child); break;
        }

    }

    void Entity::add_children(const std::vector<UUID>& children) {
        get_children().reserve(children.size());
        for (const auto& child : children) {
            add_child(child);
        }
    }

    void Entity::remove_children() {
        const auto& children = get_children();
        for (const auto& child : children) {
            remove_child(child);
        }
        remove_component<ChildrenComp>();
    }

    void Entity::remove_child(UUID child) {
        auto& children = get_children();

        DU_ASSERT(std::find(children.begin(), children.end(), child) == children.end(), "remove_child(): Entity {} is not a child of {}", child, uuid());
        Entity child_e = scene->uuid_to_entity(child);

        physically_disown_child(child_e);

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

    int get_entities_relation(Scene& scene, UUID a, UUID b) {
        bool is_a = is_ancestor_of(scene, a, b);
        bool is_b = is_ancestor_of(scene, b, a);

        return is_a - is_b;
    }





    template <typename T>
    static void clone_component(Entity& src, Entity& target) {
        if (!src.has_component<T>()) return;
        target.add_component<T>(src.get_component<T>());
    }

    static void clone_camera(Entity& src, Entity& target) {
        if (!src.has_component<CameraComp>()) return;

        const CameraComp& cam = src.get_component<CameraComp>();
        Shader tmp = {cam.present_shader.program, cam.present_shader.path};
        const glm::vec2& view_size = cam.framebuffer.last_scale;

        target.add_component<CameraComp>(
                CameraBuilder()
                .fovy(cam.fovy).pitch(cam.pitch()).yaw(cam.yaw())
                .max_distance(cam.max_distance).projection_mode(cam.projection_mode)
                .present_shader(tmp).framebuffer_size(view_size.x, view_size.y)
                .build()
                );
    }


    // FIXME: will probably crash but that's for later
    static void clone_luaaction(Scene* scene, Entity& src, Entity& target) {
        if (!src.has_component<LuaActionComp>()) return;
        const LuaActionComp& src_act = src.get_component<LuaActionComp>();

        LuaActionComp& act = target.add_component<LuaActionComp>(target.uuid());

        for (const auto& script : src_act.scripts) {
            act.add(scene,scene->get_script(script.path.c_str()));
        }
    }


    UUID clone_entity_recursively(Scene* scene, const UUID& subject, float first) {
        Entity target = scene->create_entity();
        Entity src = scene->uuid_to_entity(subject);

        clone_component<TagComp>(src, target);
        clone_component<TransformComp>(src, target);
        clone_component<PhysicsBodyComp>(src, target);
        clone_luaaction(scene, target, target);
        clone_camera(src, target);
        clone_component<ModelComp>(src, target);
        clone_component<DirLightComp>(src, target);
        clone_component<PntLightComp>(src, target);
        clone_component<SptLightComp>(src, target);

        if (src.is_child() && first) 
            target.make_child_of(src.get_parent().uuid());
        
        if (!src.is_parent()) return target.uuid();

        for (const auto& src_child : src.get_children()) {
            UUID child = clone_entity_recursively(scene, src_child, false);
            target.add_child(child);
        }

        return target.uuid(); 
    }
}
