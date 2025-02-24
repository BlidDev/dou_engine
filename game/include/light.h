#pragma once
#include <engine.h>

using namespace engine;

class LightScene : public engine::Scene {
public:
    LightScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 
private:
    bool close;
};
