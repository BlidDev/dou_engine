// src/fps_ctrl.cpp
#include "fps_ctrl.h"

void FPSController::on_init(engine::Scene* scene, engine::Entity self) {
    // capture the cursor
    engine::set_input_mode(scene, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void FPSController::on_update(engine::Scene* scene, engine::Entity self, float dt) {
    // get components
    auto& trans = self.get_component<engine::TransformComp>();
    auto& cam = self.get_component<engine::CameraComp>();


    // get input
    float f = (float)(engine::is_key_pressed(GLFW_KEY_W) - engine::is_key_pressed(GLFW_KEY_S));
    float r = (float)(engine::is_key_pressed(GLFW_KEY_D) - engine::is_key_pressed(GLFW_KEY_A));
    float u = (float)(engine::is_key_pressed(GLFW_KEY_E) - engine::is_key_pressed(GLFW_KEY_Q));

    float sensi = 2.0f;
    glm::vec2 mouse_delta = engine::get_mouse_delta() * sensi * -dt;

    // handle input
    engine::handle_mouse_delta(cam, mouse_delta, true);

    glm::vec3 forward = engine::get_camera_flat_forward(cam);
    glm::vec3 right   = engine::get_camera_right(cam);

    float speed = 3.0f;
    glm::vec3 move = (f * forward) + (r * right) + (u * cam.up);

    // apply
    trans.translate(move * speed * dt);
}


