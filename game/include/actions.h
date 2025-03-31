#pragma once
#include <engine.h>

struct SimpleAct : public engine::UpdateComp {
    bool mouse = true;

    void on_update(engine::Scene* scene, engine::Entity self, float dt);;

    virtual UpdateComp* copy();
};

