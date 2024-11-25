#pragma once
#include <espch.h>
#include "scene.h"

namespace engine {
    struct PhysicsBodyComp {
        float gravity;
        Vector3 velocity;
        Vector3 acceleration;
        bool is_solid;
        bool is_static;
        Vector3 move_delta;
        PhysicsBodyComp();
        PhysicsBodyComp(float gravity, Vector3 velocity, Vector3 acceleration, bool is_solid, bool is_static);

        int (*intersects_callback)(Scene&, entt::entity, entt::entity) = nullptr;
    };

    struct PhysicsBodyBuilder {
        PhysicsBodyBuilder() {
            physicbody = PhysicsBodyComp();
        }

        PhysicsBodyBuilder& gravity(float gravity);
        PhysicsBodyBuilder& velocity(Vector3 velocity);
        PhysicsBodyBuilder& acceleration(Vector3 acceleration);
        PhysicsBodyBuilder& is_solid(bool is_solid);
        PhysicsBodyBuilder& is_static(bool is_static);

        PhysicsBodyBuilder& intersects_callback(int (*intersects_callback)(Scene&, entt::entity, entt::entity));

        PhysicsBodyComp build();

        operator PhysicsBodyComp() {
            return this->physicbody;
        }


    private:
        PhysicsBodyComp physicbody;
    };


}
