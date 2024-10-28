#include "components/physicbody.h"


namespace engine {
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

    PhysicsBodyComp PhysicsBodyBuilder::build() {
        return this->physicbody;
    }
}
