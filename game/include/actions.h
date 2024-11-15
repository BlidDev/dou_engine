#pragma once
#include <epch.h>


struct PlayerAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
};


struct FPSAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    FPSAction(entt::entity player) : player(player){}
    entt::entity player;
};

struct CubeAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
};

struct WinCube {
    engine::SceneManager* manager;
    entt::entity player;
};

int win_cube(engine::Scene& scene,entt::entity self, entt::entity other);

struct GameCameraAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
};

struct GameAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
};

struct ObstAction : public engine::UpdateComp {
    ObstAction(bool* create, int* score, float speed);
    void on_update(entt::registry& registry, entt::entity self, float dt);
private:
    float speed = 1.0f;
    bool* create_obst;
    int* score;
};

struct ScoreAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    ScoreAction(int* score) : score(score){}
    int* score;
};
