#pragma once
#include <epch.h>


using namespace engine;


class RTScene : public Scene {
public:
    RTScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 

private:
    bool close;
    Entity player;

};
