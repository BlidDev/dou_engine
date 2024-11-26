#pragma once
#include <epch.h>

class GameScene : public engine::Scene {
public:
    GameScene();

    void on_create();

    void on_update(float dt);

    void on_end();

    bool should_close(); 

    void make_walls();
    void generate_obst();
private:
    engine::Entity player;
    bool create_obst = false, create_second = true;
    std::chrono::steady_clock::time_point begin;
    int score = 0;
    bool start = false;
};
