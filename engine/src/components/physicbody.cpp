#include "components/physicbody.h"


namespace engine {

    PhysicsBodyComp::PhysicsBodyComp() {
        gravity = 0.0f;
        velocity = {0.0f,0.0f,0.0f};
        acceleration = {0.0f,0.0f,0.0f};
        is_solid = true;
        is_static = false;
        move_delta = {0.0f,0.0f,0.0f};
        intersects_callback = nullptr;
    }

    PhysicsBodyComp::PhysicsBodyComp(float gravity, Vector3 velocity, Vector3 acceleration, bool is_solid, bool is_static) {
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
    PhysicsBodyBuilder& PhysicsBodyBuilder::velocity(Vector3 velocity) {
        physicbody.velocity = velocity;
        return *this;
    }
    PhysicsBodyBuilder& PhysicsBodyBuilder::acceleration(Vector3 acceleration) {
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
        EG_ASSERT(path.empty(), "Path given to bind_intersects_callback is empty");
        ENTT_ASSERT(function.empty(), "Function given to bind_intersects_callback is empty");
        physicbody.lua_callback = {path, function};
        return *this;
    }

    PhysicsBodyComp PhysicsBodyBuilder::build() {
        return this->physicbody;
    }
}
