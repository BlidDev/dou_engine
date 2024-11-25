#pragma once
#include "scene.h"


namespace engine {

    class Entity {
    public:
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
            EG_ASSERT(!has_component<T>(), "Tring to get non existant component {} from entity", typeid(T).name());
            return scene->registry.get<T>(entity_id);
        }

        template<typename T> 
        void remove() {
            check_null();
            EG_ASSERT(!has_component<T>(), "Trying to delete non existant component {} from entity", typeid(T).name());
            scene->registry.remove<T>(entity_id);
        }

        bool operator ! () const {
            return (entity_id == entt::null);
        }
        void terminate() {
            check_null();
            scene->uuids.erase(uuid());
            scene->registry.destroy(entity_id);
        }

        const UUID uuid() {
            return get_component<UUID>();
        }
        const entt::entity id() {
            return entity_id;
        }
        

    private:
        
        void check_null() {
            EG_ASSERT( ((entity_id == entt::null) || !scene), "Entity being used before initializing" );
        }

        entt::entity entity_id = entt::null;
        Scene* scene = nullptr;
    };
}
