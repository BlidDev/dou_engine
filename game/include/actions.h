#pragma once
#include <epch.h>


struct PlayerAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    engine::UpdateComp* copy() {return new PlayerAction;}
};


struct FPSAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    engine::UpdateComp* copy() {return new FPSAction(player);}

    FPSAction(entt::entity player) : player(player){}
    entt::entity player;
};

struct CubeAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    engine::UpdateComp* copy() {return new CubeAction;}
};

struct WinCube {
    engine::SceneManager* manager;
    entt::entity player;
};

int win_cube(engine::Scene& scene,entt::entity self, entt::entity other);

struct GameCameraAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    engine::UpdateComp* copy() {return new GameCameraAction;}
};

struct GameAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    engine::UpdateComp* copy() {return new GameAction;}
};

struct ObstAction : public engine::UpdateComp {
    ObstAction(bool* create, int* score, float speed);
    void on_update(entt::registry& registry, entt::entity self, float dt);
    engine::UpdateComp* copy() {return new ObstAction(create_obst, score, speed);}
private:
    float speed = 1.0f;
    bool* create_obst;
    int* score;
};

struct ScoreAction : public engine::UpdateComp {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    engine::UpdateComp* copy() {return new ScoreAction(score);}
    ScoreAction(int* score) : score(score){}
    int* score;
};
