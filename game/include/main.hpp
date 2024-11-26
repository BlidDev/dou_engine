#pragma once
#include <epch.h>

class MainScene : public engine::Scene {
public:
    MainScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 
private:
    engine::Entity player;
};
