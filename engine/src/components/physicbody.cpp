#include "components/physicbody.h"
#include "components/tag.h"
#include "entity.h"


namespace engine {

    PhysicsBodyComp::PhysicsBodyComp() {
        gravity = 0.0f;
        velocity = {0.0f,0.0f,0.0f};
        acceleration = {0.0f,0.0f,0.0f};
        is_solid = true;
        is_static = false;
        move_delta = {0.0f,0.0f,0.0f};
        intersects_callback = nullptr;
        dominance = Dominance::Dominant;
    }

    PhysicsBodyComp::PhysicsBodyComp(float gravity, glm::vec3 velocity, glm::vec3 acceleration, bool is_solid, bool is_static) {
        this->gravity     =  gravity     ;
        this->velocity    =  velocity    ;
        this->acceleration=  acceleration;
        this->is_solid    =  is_solid    ;
        this->is_static   =  is_static   ;
        this->acceleration = {0.0f, 0.0f, 0.0f};
        this->intersects_callback = nullptr;
    }

    PhysicsBodyBuilder& PhysicsBodyBuilder::gravity(float gravity) {
        physicbody.gravity = gravity;
        return *this;
    }
    PhysicsBodyBuilder& PhysicsBodyBuilder::velocity(glm::vec3 velocity) {
        physicbody.velocity = velocity;
        return *this;
    }
    PhysicsBodyBuilder& PhysicsBodyBuilder::acceleration(glm::vec3 acceleration) {
        physicbody.acceleration = acceleration;
        return *this;
    }
    PhysicsBodyBuilder& PhysicsBodyBuilder::is_solid(bool is_solid) {
        physicbody.is_solid = is_solid;
        return *this;
    }
    PhysicsBodyBuilder& PhysicsBodyBuilder::is_static(bool is_static) {
        physicbody.is_static = is_static;
        return *this;
    }

    PhysicsBodyBuilder& PhysicsBodyBuilder::intersects_callback(int (*intersects_callback)(Scene&, entt::entity, entt::entity)) {
        physicbody.intersects_callback = intersects_callback;
        return *this;
    }

    PhysicsBodyBuilder& PhysicsBodyBuilder::bind_intersects_callback(std::string path, std::string function) {
        DU_ASSERT(path.empty(), "Path given to bind_intersects_callback is empty");
        ENTT_ASSERT(function.empty(), "Function given to bind_intersects_callback is empty");
        physicbody.lua_callback = {path, function};
        return *this;
    }

    PhysicsBodyComp PhysicsBodyBuilder::build() {
        return this->physicbody;
    }


    void make_owned(Entity entity) {
        if (entity.has_component<PhysicsBodyComp>()) {
            entity.get_component<PhysicsBodyComp>().dominance = Dominance::Owned;
        }

        if (!entity.is_parent()) return;
        for (const auto& child : entity.get_children()) {
            make_owned(entity.scene_ptr()->uuid_to_entity(child));
        }

    }

    void make_physically_dominant(Entity entity) {
        DU_ASSERT(!entity.has_component<PhysicsBodyComp>(), "Entity {} is trying to be dominant but has no physics body", entity.uuid());
        entity.get_component<PhysicsBodyComp>().dominance = Dominance::Dominant;
        if (!entity.is_parent()) return;
        for (const auto& child : entity.get_children()) {
            make_owned(entity.scene_ptr()->uuid_to_entity(child));
        }
    }


    void physically_disown_children(Entity entity) {
        for (const auto& child : entity.get_children()) {
            Entity tmp = entity.scene_ptr()->uuid_to_entity(child);
            if (!tmp.has_component<PhysicsBodyComp>()) {
                if (!tmp.is_parent()) continue;
                physically_disown_children(tmp);
                continue;
            }
            tmp.get_component<PhysicsBodyComp>().dominance = Dominance::Dominant;

        }
    }
}
