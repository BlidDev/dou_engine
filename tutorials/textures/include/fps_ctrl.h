// include/fps_ctrl.h
#pragma once
#include <engine.h>

struct FPSController : public engine::UpdateComp {

    // optional
    void on_init(engine::Scene* scene, engine::Entity self);

    // mandatory
    void on_update(engine::Scene* scene, engine::Entity self, float dt);

    // optional
    // void on_end(engine::Scene* scene, engine::Entity self);
};
