#pragma once
#include <raylib.h>
#include <entt.hpp>
#include "scene.h"

namespace engine {
    struct PhysicsBodyComp {
        float gravity = 0.0f;
        Vector3 velocity = {0.0f,0.0f,0.0f};
        Vector3 acceleration = {0.0f,0.0f,0.0f};
        bool is_solid = true;
        bool is_static = false;
        Vector3 move_delta = {0.0f,0.0f,0.0f};

        int (*intersects_callback)(Scene&, entt::entity, entt::entity) = nullptr;
    };

    struct PhysicsBodyBuilder {
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
