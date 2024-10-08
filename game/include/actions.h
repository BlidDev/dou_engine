#pragma once
#include "engine.h"


struct PlayerAction : public engine::UpdateComponent {
    void on_update(entt::registry& registry, entt::entity self, float dt);
};


struct FPSAction : public engine::UpdateComponent {
    void on_update(entt::registry& registry, entt::entity self, float dt);
    FPSAction(entt::entity player) : player(player){}
    entt::entity player;
};

struct CubeAction : public engine::UpdateComponent {
    void on_update(entt::registry& registry, entt::entity self, float dt);
};

struct WinCube {
    engine::SceneManager* manager;
    entt::entity player;
};

int win_cube(entt::registry& registry,entt::entity self, entt::entity other);
