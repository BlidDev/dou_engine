#pragma once
#include <espch.h>
#include "scene.h"
#include "components/luascript.h"

namespace engine {

    enum Dominance {
        Owned,
        Dominant
    };

    struct PhysicsBodyComp {
        float gravity;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        bool is_solid;
        bool is_static;
        glm::vec3 move_delta;
        PhysicsBodyComp();
        PhysicsBodyComp(float gravity, glm::vec3 velocity, glm::vec3 acceleration, bool is_solid, bool is_static);

        int (*intersects_callback)(Scene&, entt::entity, entt::entity) = nullptr;
        LuaCallback lua_callback;

        Dominance dominance;
    };

    struct PhysicsBodyBuilder {
        PhysicsBodyBuilder() {
            physicbody = PhysicsBodyComp();
        }

        PhysicsBodyBuilder& gravity(float gravity);
        PhysicsBodyBuilder& velocity(glm::vec3 velocity);
        PhysicsBodyBuilder& acceleration(glm::vec3 acceleration);
        PhysicsBodyBuilder& is_solid(bool is_solid);
        PhysicsBodyBuilder& is_static(bool is_static);

        PhysicsBodyBuilder& intersects_callback(int (*intersects_callback)(Scene&, entt::entity, entt::entity));
        PhysicsBodyBuilder& bind_intersects_callback(std::string path, std::string function);

        PhysicsBodyComp build();

        operator PhysicsBodyComp() {
            return this->physicbody;
        }


    private:
        PhysicsBodyComp physicbody;
    };


    void make_owned(Entity entity);
    void make_physically_dominant(Entity entity);
    void physically_disown_children(Entity entity);

}

