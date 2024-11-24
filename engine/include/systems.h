#pragma once
#include "component.h"
#include "entity.h"
#include <entt.hpp>

namespace engine {

    void draw_primitive(engine::TransformComp& transform, engine::PrimitiveComp& pri);
    void actions(Scene* scene, float dt);
    void lua_action(Scene* scene, float dt);
    void renderer(Entity& player, entt::registry& registry);
    void physics(entt::registry& registry, float dt);
    int  aabb_check(Scene& scene, float dt);
    void end_actions(entt::registry& registry);
}
