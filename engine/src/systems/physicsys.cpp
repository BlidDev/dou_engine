#include "systems/physicsys.h"
#include "components/physicbody.h"
#include "components/transform.h"
#include "components/tag.h"
#include "entity.h"
#include "util.h"

namespace engine {

    static void velocity_damp(glm::vec3& velocity, float gravity, float mass, float slipperiness, float dt);

    static glm::vec3 adjust_vel_to_inter(AABBReturn& inter, glm::vec3 velocity);


#define PUSH_LIMIT 64
    struct PushChain {
        std::array<entt::entity, PUSH_LIMIT> chain;
        int index;
        PushChain() {
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
    static glm::vec3 get_final_velocity(glm::vec3 initial_vel, entt::entity self, TransformComp& self_transform, entt::registry& registry, PushChain& chain, float dt);



    struct UUIDEnttBind {entt::entity id; UUID uuid;};
    static int handle_callback(Scene& scene, PhysicsBodyComp& ph, UUIDEnttBind self, UUIDEnttBind other);
    static int run_callback_check(Scene& scene, entt::entity self, TransformComp& self_t, PhysicsBodyComp& self_ph);


    int physics(Scene *scene, float dt) {

        auto view = scene->registry.view<TransformComp, PhysicsBodyComp>();

        for (auto [e, t, p] : view.each()) {
            if (p.dominance == Dominance::Owned) continue; // will get handled by the parent
            glm::vec3& velocity = p.velocity;

            PushChain chain;
            bool should_gravity = get_final_velocity(glm::vec3{0.0f, velocity.y - p.gravity, 0.0f}, e, t, scene->registry, chain, dt).y != 0.0f;
            if((should_gravity || !p.is_solid) && velocity.y >= -40.0f)
                velocity.y -= p.gravity;
            velocity_damp(velocity, p.gravity, p.mass, 2.0f, dt);

            if (!p.is_solid) { // not solid, we don't need to check anything
                t.translate(velocity * dt);
                p.move_delta = velocity * dt;

                if(p.intersects_callback || p.lua_callback) {
                    if(run_callback_check(*scene, e, t, p))
                        return 1;
                }
                continue;
            }

            chain = {};
            glm::vec3 final_vel = get_final_velocity(velocity, e, t, scene->registry, chain, dt);
            t.translate(final_vel * dt);
            p.velocity = final_vel;
            p.move_delta = final_vel * dt;
        }

        return 0;
    }

    static glm::vec3 get_final_velocity(glm::vec3 initial_vel, entt::entity self, TransformComp& self_transform, entt::registry& registry, PushChain& chain, float dt) {
        if (chain.full()) return glm::vec3(0.0f);
        auto view = registry.view<TransformComp, PhysicsBodyComp>();
        glm::vec3 velocity = initial_vel;
        for (auto [e,t,p] : view.each()) {
            if (p.dominance == Dominance::Owned || !p.is_solid || 
                e == self || chain.contians(e) ||
                velocity == glm::vec3(0.0f)) continue; 

            std::string name = "";
            if(registry.any_of<TagComp>(self))
                name = registry.get<TagComp>(self).tag;
            AABBReturn inter = aabb_3d_intersects(
                    self_transform.position(),
                    velocity * dt,
                    self_transform.size(),
                    t.position(),
                    t.size());

            if (!inter) continue; // we didn't collide
            if (p.is_static) { // it's static so we can't push it
                velocity = adjust_vel_to_inter(inter, velocity);
                continue;
            }

            TransformComp tmp = t;
            glm::vec3 original_pos = t.position();

            float weight = (p.gravity == 0.0f) ? p.mass : p.gravity * p.mass;
            float resistance = weight * dt;

            float speed = glm::length(velocity);
            float new_speed = glm::max(0.0f, speed - resistance);
            glm::vec3 tmp_vel = velocity * (new_speed / speed);

            glm::vec3 returned(0.0f);
            tmp.position_ref() = original_pos + glm::vec3{tmp_vel.x, 0.0f, 0.0f} * dt;
            chain.push(self);
            returned.x = get_final_velocity(tmp_vel, e, tmp, registry, chain, dt).x;

            tmp.position_ref() = original_pos + glm::vec3{0.0f, tmp_vel.y, 0.0f} * dt;
            returned.y = get_final_velocity(tmp_vel, e, tmp, registry, chain, dt).y;

            tmp.position_ref() = original_pos + glm::vec3{0.0f, 0.0f, tmp_vel.z} * dt;
            returned.z = get_final_velocity(tmp_vel, e, tmp, registry, chain, dt).z;

            velocity = returned;
            p.velocity += returned;
            t.translate(p.velocity * dt);

        }

       return velocity;
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

    static void velocity_damp(glm::vec3& velocity, float gravity, float mass, float slipperiness, float dt) {

        bool grounded = std::abs(velocity.y) < 0.01f && gravity != 0.0f;
        
        constexpr float GROUND_DAMPING = 12.0f;
        constexpr float AIR_DAMPING = 5.5f;

        float damping = grounded ? GROUND_DAMPING : AIR_DAMPING;

        if (grounded) {
            float slip = 1.0f - slipperiness;
            damping *= slip * slip;
        }

        glm::vec2 horiz = {velocity.x, velocity.z};
        float decay = std::exp(-damping * dt);
        horiz *= decay;

        velocity.x = horiz.x;
        velocity.z = horiz.y;

        if (!grounded) {
            velocity.y *= std::exp(-AIR_DAMPING * 0.2 * dt);
        }

    }

    static glm::vec3 adjust_vel_to_inter(AABBReturn& inter, glm::vec3 velocity) {
        return {
            (inter.x) ? 0.0f : velocity.x,
            (inter.y) ? 0.0f : velocity.y,
            (inter.z) ? 0.0f : velocity.z
        };
    }
}
