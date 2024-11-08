#pragma once
#include "scene.h"
#include <utility>


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
            assert(!has_component<T>() && "ERROR: Entity already has component");
            return scene->registry.emplace<T>(entity_id,std::forward<Args>(args)...);
        }

        template<typename T>
        T& get_component() {
            check_null();
            assert(has_component<T>() && "ERROR: Trying to delete non existant component from entity");
            return scene->registry.get<T>(entity_id);
        }

        template<typename T> 
        void remove() {
            check_null();
            assert(has_component<T>() && "ERROR: Trying to delete non existant component from entity");
            scene->registry.remove<T>(entity_id);
        }

        bool operator ! () const {
            return (entity_id == entt::null);
        }
        void terminate() {
            check_null();
            scene->registry.destroy(entity_id);
        }

        const entt::entity id() {
            return entity_id;
        }
        

    private:
        
        void check_null() {
            assert( (entity_id != entt::null && scene) && "ERROR: Entity used before initializing");
        }

        entt::entity entity_id = entt::null;
        Scene* scene = nullptr;
    };
}
