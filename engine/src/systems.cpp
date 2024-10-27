#include "systems.h"
#include "component.h"
#include "macros.h"
#include "util.h"
#include "ops.hpp"
#include <algorithm>
#include <cstdio>
#include <raylib.h>

namespace engine {

    Vector3 calculate_drag(Vector3& vel, float gravity, float mul);
    void apply_drag(Vector3& vel, Vector3& drag);
    bool does_intersect_on_axis(float a, float b, float a_m, float b_m);

    void actions(entt::registry& registry, float dt) {
        auto actions = registry.view<ActionsComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.actions) {
                act->on_update(registry,entity, dt);
            }
        }
    }

    void renderer(Entity& player, entt::registry& registry) {
        auto objs = registry.view<TransformComp,PrimitiveComp>();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Camera& p_camera = player.get_component<Camera>();
        
        BeginMode3D(p_camera);
            for (auto [_, pos, pri] : objs.each()) {
                
                if (dist_vec3(pos.position, p_camera.position) < 30.0f
                || ((pri.attributes & PRIMITVE_IMMUNE) == PRIMITVE_IMMUNE))
                    draw_primitive(pos, pri);

            }


        EndMode3D();

        auto texts = registry.view<TransformComp,TextComp>();
        for (auto [_, pos, tex] : texts.each()) {
            DrawText(tex.body.c_str(), pos.position.x, pos.position.y,tex.font_size, tex.color);
        }

        EndDrawing();
    }




    
    void physics(entt::registry& registry, float dt) {
        auto objs = registry.view<TransformComp,PhysicsBodyComp>();
        for (auto [e, t, ph] : objs.each()) {
            Vector3 cvel = ph.velocity;
            Vector3 drag = calculate_drag(ph.velocity, ph.gravity, 0.4f);

            apply_drag(cvel, drag);
            cvel.y -= ph.gravity;
            

            Vector3 move_amount = cvel* dt;

            ph.velocity = cvel;
            ph.move_delta = move_amount;
        }
    }


    int aabb_check(entt::registry& registry, float dt) {
        auto objs = registry.view<TransformComp,PhysicsBodyComp>();
        for (auto [e, t, ph] : objs.each()) {
            Vector3 tp = t.position;
            if (!ph.is_solid)
            {
                t.position += ph.move_delta;
                if (ph.intersects_callback)
                {
                    for (auto [o, ot, oph] : objs.each()) {
                        if (e != o && aabb_3d_intersects(t.position, t.size, ot.position, ot.size)) {
                            if (ph.intersects_callback(registry,e,o))
                                return 1;
                        }
                    }
                }
                continue;
            }
            bool allowed_x = true, allowed_y = true, allowed_z = true;

            for (auto [o, ot, oph] : objs.each()) {
                Vector3 tmp = t.position + ph.move_delta;
                if (e == o || 
                    !oph.is_solid || 
                    dist_vec3(tmp, ot.position) > std::max({t.size.x,t.size.y,t.size.z}) + std::max({ot.size.x,ot.size.y,ot.size.z}) ||
                    !aabb_3d_intersects(t.position + ph.move_delta, t.size, ot.position, ot.size)) continue;

                tmp = Vector3{tp.x + ph.move_delta.x, tp.y, tp.z};
                HANDLE_AABB(tmp, t, ot, ph, oph, x);

                tmp = Vector3{tp.x, tp.y + ph.move_delta.y, tp.z};
                HANDLE_AABB(tmp, t, ot, ph, oph, y);

                tmp = Vector3{tp.x, tp.y, tp.z + ph.move_delta.z};
                HANDLE_AABB(tmp, t, ot, ph, oph, z);

                if(ph.intersects_callback) {
                    if (ph.intersects_callback(registry,e,o))
                        return 1;
                }
                    

            }
            t.position.x += (allowed_x) ? ph.move_delta.x : 0.0f;
            t.position.y += (allowed_y) ? ph.move_delta.y : 0.0f;
            t.position.z += (allowed_z) ? ph.move_delta.z : 0.0f;
        }

        return 0;
    }

    Vector3 calculate_drag(Vector3& vel, float gravity, float mul) {
        Vector3 res = vel * -1.0f;
        res.y = 0.0f;
        res.x *= mul;
        res.z *= mul;
        return res;
    }

    void apply_drag(Vector3& vel, Vector3& drag) {
        if (std::abs(vel.x) > std::abs(drag.x))
            vel.x += drag.x;
        else
            vel.x = 0.0f;

        if (std::abs(vel.z) > std::abs(drag.z))
            vel.z += drag.z;
        else
            vel.z = 0.0f;

    }

    void draw_primitive(engine::TransformComp& t, engine::PrimitiveComp& p) {
        bool filled = (PRIMITVE_FILLED & p.attributes) == PRIMITVE_FILLED;
        bool wireframe = (PRIMITVE_WIREFRAME & p.attributes) == PRIMITVE_WIREFRAME;
        switch(p.shape) {
            case PrimitiveComp::Shape::PLANE: {
                if (!filled) assert(0 && "ERROR: Plane primitive cannot be not filled");
                DrawPlane(t.position, Vector2{t.size.x, t.size.z}, p.color);
            }break;

            case PrimitiveComp::Shape::CUBE: {
                if(filled)
                    DrawCube(t.position, t.size.x, t.size.y, t.size.z, p.color);
                if (wireframe)
                    DrawCubeWires(t.position, t.size.x, t.size.y, t.size.z, BLACK);
             }break;

            case PrimitiveComp::Shape::SPHERE: {
                if(filled)
                    DrawSphere(t.position, t.size.x, p.color);
                if (wireframe)
                    DrawSphereWires(t.position, t.size.x * 1.01f, 8,8, BLACK);
            }break;
            default:break;
        }
    }


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
