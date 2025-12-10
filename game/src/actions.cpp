#include "actions.h"



void SimpleAct::on_update(engine::Scene* scene, engine::Entity self, float dt) {
    engine::TransformComp& trs = self.get_component<engine::TransformComp>();
    engine::PhysicsBodyComp& phy = self.get_component<engine::PhysicsBodyComp>();
    engine::CameraComp& cam = self.get_component<engine::CameraComp>();


    if(mouse)
        engine::update_camera_target(cam, trs.position());
    float speed = 0.5f;
    float f = (float)(engine::is_key_pressed(GLFW_KEY_W) - engine::is_key_pressed(GLFW_KEY_S));
    float r = (float)(engine::is_key_pressed(GLFW_KEY_D) - engine::is_key_pressed(GLFW_KEY_A));

    if (engine::is_key_pressed(GLFW_KEY_SPACE) && phy.move_delta.y == 0.0f) phy.velocity.y += 10.0f;

    bool affected = phy.gravity > 0.0f;


    if (engine::is_key_pressed(GLFW_KEY_LEFT_CONTROL)) {phy.gravity = 0.0f; phy.velocity.y = 0.0f;}
    if (engine::is_key_clicked(GLFW_KEY_LEFT_ALT)) {
        if(affected){
            phy.gravity = 0.0f;
            phy.velocity.y = 0.0f;
            engine::DU_INFO("Off");
        }
        else {
            phy.gravity = 0.2f;
            engine::DU_INFO("On");
        }
    }

    if (engine::is_key_clicked(GLFW_KEY_F)) {
        mouse = !mouse;
        int mode = (mouse) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
        glfwSetInputMode(scene->manager->main_window, GLFW_CURSOR, mode);
    }


    speed = (engine::is_key_pressed(GLFW_KEY_LEFT_SHIFT)) ? speed * 3.0f : speed;

    auto pos = trs.position();
    glm::vec3 forward = engine::get_flat_forward(cam.target, pos) * 3.0f;
    glm::vec3 right   = engine::get_right(cam.target, pos, cam.up) * 3.0f;

    glm::vec2 mouse_delta = engine::get_mouse_delta() * -0.05f;

    if (mouse)
        engine::handle_mouse_delta(&cam, pos, mouse_delta, true);

    glm::vec3 move = (forward * f) + (right * r);
    move.y = 0.0f;
    phy.velocity += move * speed;
}

engine::UpdateComp* SimpleAct::copy()  {
    return this;
}
