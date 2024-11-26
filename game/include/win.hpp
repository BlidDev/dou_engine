#pragma once
#include <epch.h>

class WinScene : public engine::Scene {
public:
    WinScene();

    void on_create();

    void on_update(float dt);

    bool should_close();

    void on_end();
    entt::entity text;

};
