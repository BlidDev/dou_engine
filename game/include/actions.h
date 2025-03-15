#pragma once
#include <engine.h>

struct SimpleAct : public engine::UpdateComp {
    void on_update(engine::Scene* scene, engine::Entity self, float dt) {
        engine::PhysicsBodyComp& phy = self.get_component<engine::PhysicsBodyComp>();
        engine::CameraComp& cam = self.get_component<engine::CameraComp>();

        if (engine::is_key_pressed(GLFW_KEY_D))
            phy.velocity.x += 0.5f;
        if (engine::is_key_pressed(GLFW_KEY_A))
            phy.velocity.x -= 0.5f;

        if (engine::is_key_pressed(GLFW_KEY_W))
            phy.velocity.z += 0.5f;
        if (engine::is_key_pressed(GLFW_KEY_S))
            phy.velocity.z -= 0.5f;
    }

    virtual UpdateComp* copy()  {
        return this;
    }
};

