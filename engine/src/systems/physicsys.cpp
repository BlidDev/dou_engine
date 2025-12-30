#include "systems/physicsys.h"
#include "components/physicbody.h"
#include "components/transform.h"
#include "components/tag.h"
#include "entity.h"
#include "util.h"
#include "formatting.h"

namespace engine {
    #define MIN_SPEED 0.001f

    enum class Axis {X = 0,Y,Z};
    static void velocity_damp(glm::vec3& velocity, float gravity, float slipperiness, float dt);
    static glm::vec3 adjust_vel_to_inter(AABBReturn& inter, const glm::vec3& velocity);


    #define PUSH_LIMIT 64
    struct PushChain {
        std::array<entt::entity, PUSH_LIMIT> chain;
        int index;
        PushChain() {
            chain.fill(entt::null);
            index = 0;
        }
        void clear() {
            chain.fill(entt::null);
            index = 0;
        }
        void push(entt::entity value) {
            DU_ASSERT(full(), "Push limited reached");
            chain[index++] = value;
        }
        bool contians(entt::entity value) {
            return std::find(chain.begin(), chain.end(), value) != chain.end();
        }
        bool full() {
            return !(chain[PUSH_LIMIT -1] == entt::null);
        }
        int size() {
            return index;
        }
    };

    static void apply_translation_on_children(Scene& scene, UUID current, glm::vec3 move_delta, float dt, bool translate = true, bool absolute = true);
    static float get_final_axis_vel(Scene& scene, float initial_vel, Axis axis, entt::entity self, TransformComp& self_transform, float self_mass, PushChain& chain, float dt);


    struct UUIDEnttBind {entt::entity id; UUID uuid;};
    static int handle_callback(Scene& scene, PhysicsBodyComp& ph, UUIDEnttBind self, UUIDEnttBind other);
    static int run_callback_check(Scene& scene, entt::entity self, TransformComp& self_t, PhysicsBodyComp& self_ph);


    int physics(Scene *scene, float dt) {

        auto view = scene->registry.view<UUID,TransformComp, PhysicsBodyComp>();

        for (auto [e, u, t, p] : view.each()) {
            if (p.dominance == Dominance::Owned) {
                if(p.intersects_callback || p.lua_callback) {
                    if(run_callback_check(*scene, e, t, p))
                        return 1;
                }
                continue;
            }

            glm::vec3& velocity = p.velocity;

            PushChain chain;
            float final = get_final_axis_vel(*scene, velocity.y - p.gravity, Axis::Y, e, t, p.mass, chain, dt);
            bool grounded = final == 0.0f;
            if((!grounded || !p.is_solid) && velocity.y >= -40.0f && p.gravity != 0.0f) 
                velocity.y -= p.gravity * dt;

            velocity_damp(velocity, p.gravity, p.slipperiness, dt);

            if (!p.is_solid) { // not solid, we don't need to check anything
                t.translate(velocity * dt);
                p.move_delta = velocity * dt;
                apply_translation_on_children(*scene, u, velocity, dt, false);

                if(p.intersects_callback || p.lua_callback) {
                    if(run_callback_check(*scene, e, t, p))
                        return 1;
                }
                continue;
            }

            chain.clear();
            velocity.x = get_final_axis_vel(*scene, velocity.x, Axis::X, e, t, p.mass, chain, dt);
            chain.clear();
            velocity.y = get_final_axis_vel(*scene, velocity.y, Axis::Y, e, t, p.mass, chain, dt);
            chain.clear();
            velocity.z = get_final_axis_vel(*scene, velocity.z, Axis::Z, e, t, p.mass, chain, dt);

            t.translate(velocity * dt);
            p.move_delta = velocity * dt;
            apply_translation_on_children(*scene, u, velocity, dt, true);
        }

        return 0;
    }

    int fixed_physics(Scene *scene, size_t target_fps, float dt) {
        static float accumulated = 0.0f;
        accumulated += dt;
        float target_dt = 1.0f / target_fps;

        int result = 0;

        if (accumulated >= target_dt) {
            result =  physics(scene, target_dt);
            accumulated -= target_dt;
        }
        return result;
    }




    static float get_final_axis_vel(Scene& scene, float initial_vel, Axis axis, entt::entity self, TransformComp& self_transform, float self_mass, PushChain& chain, float dt) {
        float vel = initial_vel;
        if (std::abs(vel) <= MIN_SPEED) return 0.0f; 

        auto view = scene.registry.view<UUID,TransformComp, PhysicsBodyComp>();
        
        for (auto [e,u,t,p] : view.each()) {
            if (p.dominance == Dominance::Owned || !p.is_solid || 
                e == self || chain.contians(e)) continue;  

            glm::vec3 delta(0.0f);
            delta[(int)axis] = vel * dt;

            AABBReturn inter = aabb_3d_intersects(
                    self_transform.position(),
                    delta,
                    self_transform.size(),
                    t.position(),
                    t.size());

            if (!inter)
                continue;

            if (p.is_static) {
                delta = adjust_vel_to_inter(inter, delta);
                vel = delta[(int)axis];
                apply_translation_on_children(scene, scene.registry.get<UUID>(self), delta, dt);
                continue;
            }

            float scale = 10.0f;
            float gravity_term = glm::max(p.gravity, 0.0f) / scale;
            float resistance = p.mass * (1.0f + gravity_term);

            float whole = self_mass + resistance;
            float tmp_vel = vel * (self_mass / whole);


            chain.push(self);
            float returned = get_final_axis_vel(scene, tmp_vel, axis, e, t, p.mass, chain, dt);

            vel = returned;
         
            p.velocity[(int)axis] += returned * dt;
            glm::vec3 push_delta(0.0f); 
            push_delta[(int)axis] = vel;
            t.translate(push_delta * dt);
        }

        return vel;
    }

    static void apply_translation_on_children(Scene& scene, UUID current, glm::vec3 move_delta, float dt, bool translate, bool absolute) {
        Entity tmp = scene.uuid_to_entity(current);
        if (!tmp.is_parent()) return; 

        for (const auto& child_uuid : tmp.get_children()) {
            Entity child = scene.uuid_to_entity(child_uuid);
            if (child.has_component<TransformComp>() && translate)
                child.get_component<TransformComp>().translate(move_delta * dt);
            
            if (child.has_component<PhysicsBodyComp>()) {
                auto& velocity = child.get_component<PhysicsBodyComp>().velocity;
                velocity = ((!absolute) ? velocity : glm::vec3(0.0f)) + move_delta;
            }

            apply_translation_on_children(scene, child_uuid, move_delta, dt);
        }
    }


    static int run_callback_check(Scene& scene, entt::entity self, TransformComp& self_t, PhysicsBodyComp& self_ph) {
        auto view = scene.registry.view<UUID, TransformComp, PhysicsBodyComp>();
        UUID self_uuid = view.get<UUID>(self);
        for (auto [other, uuid, t, p] : view.each()) {
            if (other == self) continue;
            if (get_entities_relation(scene, self_uuid, uuid) != 0) continue; // related
            if (!does_aabb_intersects(self_t.position(), self_t.size(), t.position(), t.size())) continue; // no collision

            if(handle_callback(scene, self_ph, {self, self_uuid}, {other, uuid}))
                return 1;
        }

        return 0;
    }

    static int handle_callback(Scene& scene, PhysicsBodyComp& ph, UUIDEnttBind self, UUIDEnttBind other) {
        if(ph.intersects_callback != nullptr) {
            return ph.intersects_callback(scene, self.id, other.id);
        }
        else if (ph.lua_callback) {
            DU_ASSERT(!scene.registry.any_of<LuaActionComp>(self.id), 
                    "LuaCallback is set to ({}, {}) but entity does not have any scripts attached", ph.lua_callback.path, ph.lua_callback.function);
            LuaActionComp& lua_a = scene.registry.get<LuaActionComp>(self.id);
            return lua_a.call_at(ph.lua_callback, self.uuid, other.uuid);
        }


        return 0;
    }

    static void velocity_damp(glm::vec3& velocity, float gravity, float slipperiness, float dt) {
        if (velocity == glm::vec3(0.0f)) return;
        bool grounded = std::abs(velocity.y) <= MIN_SPEED && gravity != 0.0f;

        glm::vec2 horiz = {velocity.x, velocity.z};
        float damp = slipperiness * (!grounded ? 2.0f : 1.0f);
        float decay = std::pow(std::clamp(damp,0.0f, 1.0f), dt);
        horiz *= decay;

        velocity.x = (std::abs(horiz.x) > MIN_SPEED) ? horiz.x : 0.0f;
        velocity.z = (std::abs(horiz.y) > MIN_SPEED) ? horiz.y : 0.0f;

        constexpr float AIR_DAMPING = 0.2f;
        if (!grounded) {
            float y_decay = std::exp(AIR_DAMPING * 0.2 * dt);
            velocity.y *= (std::abs(y_decay) > MIN_SPEED) ? y_decay : 0.0f;
        }

    }

    static glm::vec3 adjust_vel_to_inter(AABBReturn& inter, const glm::vec3& velocity) {
        return {
            (inter.x) ? 0.0f : velocity.x,
            (inter.y) ? 0.0f : velocity.y,
            (inter.z) ? 0.0f : velocity.z
        };
    }
}
