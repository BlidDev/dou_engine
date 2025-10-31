#pragma once
#include "scene.h"

namespace engine {

    class Entity {
    public:
        static Entity null() {
            return Entity(nullptr, entt::null);
        }
        Entity() = default;
        Entity(Scene* scene, entt::entity id);

        template<typename T>
        bool has_component() {
            check_null();
            return scene->registry.any_of<T>(entity_id);
        }

        template<typename T, typename ...Args>
        T& add_component(Args&&... args) {
            check_null();
            EG_ASSERT(has_component<T>(),"Entity already has component {}", typeid(T).name());
            return scene->registry.emplace<T>(entity_id,std::forward<Args>(args)...);
        }

        template<typename T>
        T& get_component() {
            check_null();
            EG_ASSERT(!has_component<T>(), "Tring to get non existant component {} from entity {}", typeid(T).name(), (uint64_t)uuid());
            return scene->registry.get<T>(entity_id);
        }

        template<typename T> 
        void remove_component() {
            check_null();
            EG_ASSERT(!has_component<T>(), "Trying to delete non existant component {} from entity", typeid(T).name());
            scene->registry.remove<T>(entity_id);
        }


        bool is_child();
        bool is_parent();
        Entity& get_parent();
        std::vector<UUID>& get_children();

        void make_child_of(UUID parent);
        void add_child(UUID child);
        void add_children(std::vector<UUID> children);
        void remove_child(UUID child);
        void remove_children();
        void remove_parent();

        operator bool() const {
            return (entity_id != entt::null) || (scene == nullptr);
        }

        bool operator==(Entity& other) {
            return uuid() == other.uuid();
        }

        void terminate() {
            check_null();
            scene->uuids.erase(uuid());
            scene->registry.destroy(entity_id);
        }

        UUID uuid() {
            return get_component<UUID>();
        }
        entt::entity id() {
            return entity_id;
        }

        Scene* scene_ptr() {
            EG_ASSERT(!scene, "Entity {} has no scene attached", uuid());
            return scene;
        }
        

    private:
        
        void check_null() {
            EG_ASSERT( ((entity_id == entt::null) || !scene), "Entity being used before initializing" );
        }

        entt::entity entity_id = entt::null;
        Scene* scene = nullptr;
    };


    bool is_ancestor_of(Scene& scene, UUID ancestor, UUID subject);
}


