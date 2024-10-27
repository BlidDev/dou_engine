#include "actions.h"
#include "util.h"
#include "ops.hpp"
#include <raylib.h>


void PlayerAction::on_update(entt::registry& registry, entt::entity self, float dt) {

    Camera& p_camera = registry.get<Camera>(self);
    engine::TransformComp& p_t = registry.get<engine::TransformComp>(self);
    engine::PhysicsBodyComp& p_ph = registry.get<engine::PhysicsBodyComp>(self);
    float speed = 1.0f; 
    if (IsKeyDown(KEY_LEFT_SHIFT)) speed *= 3.0f;
    if (IsKeyDown(KEY_SPACE) && p_ph.move_delta.y == 0.0f) p_ph.velocity.y += 10.0f;

    if (IsKeyDown(KEY_LEFT_CONTROL)) { p_ph.gravity = 0.0f; }
    if (IsKeyDown(KEY_LEFT_ALT)) { p_ph.gravity = 0.2f; }

    p_camera.target += p_t.position - p_camera.position;
    p_camera.position = p_t.position;
    engine::handle_mouse_delta(&p_camera, {
                GetMouseDelta().x * 0.05f,
                GetMouseDelta().y * 0.05f, 
                0.0f
            }, true);

    Vector3 forward = engine::get_forward(p_camera.target, p_t.position) * 3.0f;
    Vector3 right   = engine::get_right(p_camera.target, p_t.position, p_camera.up) * 3.0f;

    int f = IsKeyDown(KEY_W) - (IsKeyDown(KEY_S));
    int r = IsKeyDown(KEY_D) - (IsKeyDown(KEY_A));

    Vector3 move = (forward * f) + (right * r);
    move.y = 0.0f;
    p_ph.velocity += move * speed;
}


void FPSAction::on_update(entt::registry& registry, entt::entity self, float dt) {
    engine::TextComp& text = registry.get<engine::TextComp>(self);
    engine::TransformComp& p = registry.get<engine::TransformComp>(player);

    text.body = TextFormat("FPS: %d | (%f. %f, %f)", GetFPS(),p.position.x, p.position.y, p.position.z);
}

void CubeAction::on_update(entt::registry &registry, entt::entity self, float dt) {
    engine::PhysicsBodyComp& physics = registry.get<engine::PhysicsBodyComp>(self);

    physics.velocity.z = sin(GetTime() +  (float)self) * 0.5f + 0.5f;
    physics.velocity.x = sin(GetTime() +  (float)self);
}


int win_cube(entt::registry& registry,entt::entity self, entt::entity other) {
    auto& win_cube = registry.get<WinCube>(self);
    if (win_cube.player == other){
        win_cube.manager->set_current("win");
        return 1;
    }

    return 0;
}
