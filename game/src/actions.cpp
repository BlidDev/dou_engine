#include "actions.h"
#include "util.h"
#include "ops.hpp"


void PlayerAction::on_update(engine::Scene* scene, engine::Entity self, float dt) {

    Camera& p_camera = self.get_component<Camera>();
    engine::TransformComp& p_t = self.get_component<engine::TransformComp>();
    engine::PhysicsBodyComp& p_ph = self.get_component<engine::PhysicsBodyComp>();
    float speed = 0.5f; 
    if (IsKeyDown(KEY_LEFT_SHIFT)) speed *= 3.0f;
    if (IsKeyDown(KEY_SPACE) && p_ph.move_delta.y == 0.0f) p_ph.velocity.y += 10.0f;

    if (IsKeyDown(KEY_LEFT_CONTROL)) { p_ph.gravity = 0.0f; }
    if (IsKeyDown(KEY_LEFT_ALT)) { p_ph.gravity = 0.2f; }

    p_camera.target += p_t.position - p_camera.position;
    p_camera.position = p_t.position;
    engine::handle_mouse_delta(&p_camera, {
                GetMouseDelta().x * 0.05f,
                GetMouseDelta().y * 0.05f}
            , true);

    Vector3 forward = engine::get_forward(p_camera.target, p_t.position) * 3.0f;
    Vector3 right   = engine::get_right(p_camera.target, p_t.position, p_camera.up) * 3.0f;

    int f = IsKeyDown(KEY_W) - (IsKeyDown(KEY_S));
    int r = IsKeyDown(KEY_D) - (IsKeyDown(KEY_A));

    Vector3 move = (forward * f) + (right * r);
    move.y = 0.0f;
    p_ph.velocity += move * speed;
}


void FPSAction::on_update(engine::Scene* scene, engine::Entity self, float dt) {
    engine::TextComp& text = self.get_component<engine::TextComp>();
    engine::TransformComp& p = scene->get_uuid_component<engine::TransformComp>(player);

    text.body = TextFormat("FPS: %d | (%f. %f, %f)", GetFPS(),p.position.x, p.position.y, p.position.z);
}

void CubeAction::on_update(engine::Scene* scene, engine::Entity self, float dt) {
    engine::PhysicsBodyComp& physics = self.get_component<engine::PhysicsBodyComp>();

    physics.velocity.z = sin(GetTime() +  (float)self.id()) * 0.5f;
    physics.velocity.x = sin(GetTime() +  (float)self.id());
}


int win_cube(engine::Scene& scene,entt::entity self, entt::entity other) {
    auto& win_cube = scene.registry.get<WinCube>(self);
    if (win_cube.player == other){
        win_cube.manager->set_current("win");
        return 1;
    }

    return 0;
}

void GameCameraAction::on_update(engine::Scene* scene, engine::Entity self, float dt) {
    Camera& p_camera = self.get_component<Camera>();
    engine::TransformComp& p_t = self.get_component<engine::TransformComp>();

    p_camera.target += p_t.position - p_camera.position;
    p_camera.position = p_t.position;
}

void GameAction::on_update(engine::Scene* scene, engine::Entity self, float dt) {

    engine::PhysicsBodyComp& p_ph = self.get_component<engine::PhysicsBodyComp>();
    float speed = 5.0f; 
    int v = IsKeyDown(KEY_UP) - (IsKeyDown(KEY_DOWN));
    int h = IsKeyDown(KEY_RIGHT) - (IsKeyDown(KEY_LEFT));

    p_ph.velocity.x += -(float)h * speed;
    p_ph.velocity.y  =  (float)v * speed;
}

ObstAction::ObstAction(bool* create, int* score, float speed) {
    this->speed = speed;
    this->score = score;
    this->create_obst = create; 
}

void ObstAction::on_update(engine::Scene* scene, engine::Entity self, float dt) {
    engine::PhysicsBodyComp& ph = self.get_component<engine::PhysicsBodyComp>();
    engine::TransformComp& t = self.get_component<engine::TransformComp>();

    ph.velocity.z = speed;

    if (t.position.z <= -1.0f) {
        self.terminate();
        *create_obst = true;
    }
}


void ScoreAction::on_update(engine::Scene* scene, engine::Entity self, float dt) {

    engine::TextComp& text = self.get_component<engine::TextComp>();

    text.body = TextFormat("FPS: %d | SCORE: %d", GetFPS(), *score);
}
