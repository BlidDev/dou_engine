
#pragma once
#include <engine.h>


class ThingScene : public engine::Scene {
public:
    ThingScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 
private:
    bool close;
    engine::Entity player;
};
