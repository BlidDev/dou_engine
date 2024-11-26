#pragma once
#include <epch.h>

using namespace engine;

class TestScene : public engine::Scene {
public:
    TestScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 
};
