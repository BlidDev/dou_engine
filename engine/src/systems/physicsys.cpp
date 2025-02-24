#include "systems/physicsys.h"
#include "util.h"
#include "macros.h"
#include "components/transform.h"
#include "components/physicbody.h"
#include "components/luascript.h"


namespace engine {

    glm::vec3 calculate_drag(glm::vec3& vel, float gravity, float mul);
    void apply_drag(glm::vec3& vel, glm::vec3& drag);
    bool does_intersect_on_axis(float a, float b, float a_m, float b_m);

    void physics(entt::registry& registry, float dt) {
        auto objs = registry.view<TransformComp,PhysicsBodyComp>();
        for (auto [e, t, ph] : objs.each()) {
            glm::vec3 cvel = ph.velocity;
            glm::vec3 drag = calculate_drag(ph.velocity, ph.gravity, 0.4f);

            apply_drag(cvel, drag);
            cvel.y -= ph.gravity;
            

            glm::vec3 move_amount = cvel* dt;

            ph.velocity = cvel;
            ph.move_delta = move_amount;
            if (!ph.is_solid)
                t.position += move_amount;
        }
    }

    int handle_callback(Scene& scene, PhysicsBodyComp& ph, entt::entity e, entt::entity o) {
        if(ph.intersects_callback != nullptr) {
            if(ph.intersects_callback(scene, e, o))
                return 1;
        }
        else if (ph.lua_callback) {
            EG_ASSERT(!scene.registry.any_of<LuaActionComp>(e), 
                    "LuaCallback is set to ({}, {}) but entity does not have any scripts attached", ph.lua_callback.path, ph.lua_callback.function);
            LuaActionComp lua_a = scene.registry.get<LuaActionComp>(e);
            UUID eu = scene.registry.get<UUID>(e);
            UUID ou = scene.registry.get<UUID>(o);
            if (lua_a.call_at(ph.lua_callback, eu, ou))
                return 1;
        }


        return 0;
    }

    int aabb_check(Scene& scene, float dt) {
        auto objs = scene.registry.view<TransformComp,PhysicsBodyComp>();
        bool allowed_x = true, allowed_y = true, allowed_z = true;
        for (auto [e, t, ph] : objs.each()) {
            glm::vec3 tp = t.position;
            for (auto [o, ot, oph] : objs.each()) {
                glm::vec3 tmp = t.position + ph.move_delta;
                if (e == o || 
                    dist_vec3(tmp, ot.position) > std::max({t.size.x,t.size.y,t.size.z}) + std::max({ot.size.x,ot.size.y,ot.size.z}) ||
                    !aabb_3d_intersects(t.position + ph.move_delta, t.size, ot.position, ot.size)) continue;
                if (ph.is_solid && oph.is_solid) {
                    tmp = glm::vec3{tp.x + ph.move_delta.x, tp.y, tp.z};
                    HANDLE_AABB(tmp, t, ot, ph, oph, x);

                    tmp = glm::vec3{tp.x, tp.y + ph.move_delta.y, tp.z};
                    HANDLE_AABB(tmp, t, ot, ph, oph, y);

                    tmp = glm::vec3{tp.x, tp.y, tp.z + ph.move_delta.z};
                    HANDLE_AABB(tmp, t, ot, ph, oph, z);
                }

                if(ph.intersects_callback || ph.lua_callback) {
                    if (handle_callback(scene,ph,e,o)) {
                        return 1;
                    }
                }
                    

            }
            t.position.x += (allowed_x) ? ph.move_delta.x : 0.0f;
            t.position.y += (allowed_y) ? ph.move_delta.y : 0.0f;
            t.position.z += (allowed_z) ? ph.move_delta.z : 0.0f;
        }

        return 0;
    }

    glm::vec3 calculate_drag(glm::vec3& vel, float gravity, float mul) {
        glm::vec3 res = vel * -1.0f;
        res.y = 0.0f;
        res.x *= mul;
        res.z *= mul;
        return res;
    }

    void apply_drag(glm::vec3& vel, glm::vec3& drag) {
        if (std::abs(vel.x) > std::abs(drag.x))
            vel.x += drag.x;
        else
            vel.x = 0.0f;

        if (std::abs(vel.z) > std::abs(drag.z))
            vel.z += drag.z;
        else
            vel.z = 0.0f;

    }

    bool does_intersect_on_axis(float a, float b, float a_s, float b_s) {

        bool res =  a <= b + b_s &&
                    a + a_s >= b;
        return res;
    }
}
