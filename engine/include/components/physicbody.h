#pragma once
#include <espch.h>
#include "scene.h"
#include "components/luascript.h"

namespace engine {

    enum Dominance {
        Owned,
        Dominant
    };

    using NativeCallback = int (*)(Scene&, Entity, Entity);

    struct PhysicsBodyComp {
        float gravity;
        glm::vec3 velocity;
        float mass;
        float slipperiness;
        bool is_solid;
        bool is_static;
        glm::vec3 move_delta;
        PhysicsBodyComp();
        PhysicsBodyComp(float gravity, float mass, glm::vec3 velocity, bool slipperiness, bool is_solid, bool is_static);

        bool is_dominant();

        NativeCallback intersects_callback= nullptr;
        LuaCallback lua_callback;

        Dominance dominance;
    };

    struct PhysicsBodyBuilder {
        PhysicsBodyBuilder() {
            physicbody = PhysicsBodyComp();
        }

        PhysicsBodyBuilder& gravity(float gravity);
        PhysicsBodyBuilder& velocity(glm::vec3 velocity);
        PhysicsBodyBuilder& mass(float mass);
        PhysicsBodyBuilder& slipperiness(float slipperiness);
        PhysicsBodyBuilder& is_solid(bool is_solid);
        PhysicsBodyBuilder& is_static(bool is_static);

        /** 
         * Native intersection callback 
         * @intersects_callback: int fn(Scene&, Entity self, Entity other) function pointer
         */ 
        PhysicsBodyBuilder& intersects_callback(NativeCallback intersects_callback);

        /** 
         * Lua intersection callback
         * @path: Path of an ALREADY registered and attached script
         * @name: callback function name (int fn(UUID self, UUID other) expected)
         */ 
        PhysicsBodyBuilder& bind_intersects_callback(std::string path, std::string function);

        PhysicsBodyComp build();

        operator PhysicsBodyComp() {
            return this->physicbody;
        }


    private:
        PhysicsBodyComp physicbody;
    };


    void make_physically_owned(Entity entity);
    void make_physically_dominant(Entity entity);
    void physically_disown_child(Entity child);

    Dominance get_tree_dominance(Entity& entity);

}

