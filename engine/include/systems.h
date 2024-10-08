#pragma once
#include "component.h"
#include "entity.h"
#include <entt.hpp>

namespace engine {

    void draw_primitive(engine::Transform& transform, engine::Primitive& pri);
    void actions(entt::registry& registry, float dt);
    void renderer(Entity& player, entt::registry& registry);
    void physics(entt::registry& registry, float dt);
    int  aabb_check(entt::registry& registry, float dt);
    void end_actions(entt::registry& registry);
}
