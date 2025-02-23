#include <entt.hpp>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "systems.h"
#include "macros.h"
#include "util.h"
#include "component.h"

namespace engine {

    glm::vec3 calculate_drag(glm::vec3& vel, float gravity, float mul);
    void apply_drag(glm::vec3& vel, glm::vec3& drag);
    bool does_intersect_on_axis(float a, float b, float a_m, float b_m);

    void actions(Scene* scene, float dt) {
        auto actions = scene->registry.view<ActionsComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.actions) {
                Entity e = {scene, entity};
                act->on_update(scene, e, dt);
            }
        }
    }

    void lua_action_init(Scene* scene) {

        auto actions = scene->registry.view<LuaActionComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.scripts) {
                act.on_init();
            }
        }
    }

    void lua_action_update(Scene* scene, float dt) {

        auto actions = scene->registry.view<LuaActionComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.scripts) {
                act.on_update(dt);
            }
        }
    }

    void lua_action_end(Scene* scene) {

        auto actions = scene->registry.view<LuaActionComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.scripts) {
                act.on_end();
            }
        }
    }


    void opengl_renderer(entt::registry& registry) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT );

        //Camera& p_camera = player.get_component<Camera>();
        auto objects = registry.view<TransformComp, ModelComp>();

        for (auto [_, pos, obj] : objects.each()) {
            glUseProgram(obj.material.shader);
            glBindVertexArray(obj.model.VAO);
            if ((obj.material.attributes & MODEL_FILLED) == MODEL_FILLED)
                glDrawArrays(GL_TRIANGLES, 0, 3);
            if((obj.material.attributes & MODEL_WIREFRAME) == MODEL_WIREFRAME) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        glDrawArrays(GL_TRIANGLES, 0,3);
                    }
            }

    }




    
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

    //void draw_primitive(engine::TransformComp& t, engine::PrimitiveComp& p) {
    //    bool filled = (PRIMITVE_FILLED & p.attributes) == PRIMITVE_FILLED;
    //    bool wireframe = (PRIMITVE_WIREFRAME & p.attributes) == PRIMITVE_WIREFRAME;
    //    switch(p.shape) {
    //        case PrimitiveComp::Shape::PLANE: {
    //            EG_ASSERT(!filled, "Plane primitve mut be filled");
    //            DrawPlane(t.position, Vector2{t.size.x, t.size.z}, p.color);
    //        }break;

    //        case PrimitiveComp::Shape::CUBE: {
    //            if(filled)
    //                DrawCube(t.position, t.size.x, t.size.y, t.size.z, p.color);
    //            if (wireframe)
    //                DrawCubeWires(t.position, t.size.x, t.size.y, t.size.z, BLACK);
    //         }break;

    //        case PrimitiveComp::Shape::SPHERE: {
    //            if(filled)
    //                DrawSphere(t.position, t.size.x, p.color);
    //            if (wireframe)
    //                DrawSphereWires(t.position, t.size.x * 1.01f, 8,8, BLACK);
    //        }break;
    //        default:break;
    //    }
    //}


    void end_actions(entt::registry& registry) {
        auto actions = registry.view<ActionsComp>();

        for (auto [_, act] : actions.each()) {
            for (auto* u : act.actions) {
                delete u;
            }
        }
    }

    bool does_intersect_on_axis(float a, float b, float a_s, float b_s) {

        bool res =  a <= b + b_s &&
                    a + a_s >= b;
        return res;
    }
}
