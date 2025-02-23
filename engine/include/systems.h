#pragma once
#include "scene.h"
#include "entity.h"

namespace engine {

    void actions(Scene* scene, float dt);

    void lua_action_init(Scene* scene);
    void lua_action_update(Scene* scene, float dt);
    void lua_action_end(Scene* scene);
    
    //void renderer(Entity& player, entt::registry& registry);
    void opengl_renderer(entt::registry& registry);
    void physics(entt::registry& registry, float dt);
    int  aabb_check(Scene& scene, float dt);
    void end_actions(entt::registry& registry);
}
