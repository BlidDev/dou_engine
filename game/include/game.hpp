#pragma once
#include "engine.h"
#include "actions.h"
#include <raylib.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

char obst1[] {
    '#', '#', ' ', '#', '#', 
    '#', '#', ' ', '#', '#', 
    ' ', ' ', ' ', ' ', ' ', 
    '#', '#', ' ', '#', '#', 
    '#', '#', ' ', '#', '#'
};

char obst2[] {
    '#', ' ', '#', ' ', '#', 
    ' ', '#', ' ', '#', ' ', 
    '#', ' ', '#', ' ', '#', 
    ' ', '#', ' ', '#', ' ', 
    '#', ' ', '#', ' ', '#'
};
char obst3[] {
    ' ', '#', '#', '#', '#', 
    '#', ' ', '#', '#', '#', 
    '#', '#', ' ', '#', '#', 
    '#', '#', '#', ' ', '#', 
    '#', '#', '#', '#', ' '
};

char obst4[] {
    '#', '#', '#', '#', ' ', 
    '#', '#', '#', ' ', '#', 
    '#', '#', ' ', '#', '#', 
    '#', ' ', '#', '#', '#', 
    ' ', '#', '#', '#', '#'
};
char obst5[] {
    '#', '#', '#', '#', '#', 
    '#', '#', ' ', '#', '#', 
    '#', '#', '#', '#', '#', 
    '#', '#', '#', '#', '#', 
    '#', '#', '#', '#', '#'
};

char* obsts[] = {obst1, obst2, obst3, obst4, obst5};


using namespace engine;

int intersects_callback(Scene& scene,entt::entity self, entt::entity other);

class GameScene : public engine::Scene {
public:
    GameScene() : Scene() {}

    void on_create() {
        begin = std::chrono::steady_clock::now();
        create_obst = false;
        create_second = true;
        score = 0;
        start = false;
        DisableCursor();
        player = create_entity(); 

        player = create_entity();
        player.add_component<TransformComp>(TransformBuilder().position({ 2.0f, 2.0f, 0.0f }).size({0.1f, 0.1f, 0.1f}));
        player.add_component<Camera>(CameraBuilder().position({ 2.0f, 2.0f, 0.0f }).target({ 2.0f, 2.0f, 2.0f }).build());
        player.add_component<ActionsComp>(ActionsComp().add(new GameAction).add(new GameCameraAction));
        player.add_component<PhysicsBodyComp>();
        player.add_component<TagComp>("Player");

        Entity text = create_entity();
        text.add_component<TransformComp>(TransformBuilder().position({ 0.0f, 0.0f, 0.0f }));
        text.add_component<TextComp>("", 40, BLACK);
        text.add_component<ActionsComp>(ActionsComp().add(new ScoreAction(&score)));
        
        Entity tmp = create_entity();
        tmp.add_component<TransformComp>(TransformBuilder().position({ 2.0f, 2.0f, 0.5f }).size({0.1f, 0.1f, 0.1f}));
        tmp.add_component<ActionsComp>(ActionsComp().add(new GameAction));
        tmp.add_component<PrimitiveComp>(
                PrimitiveComp::CUBE, 
                DARKGREEN, PRIMITVE_WIREFRAME );
        tmp.add_component<PhysicsBodyComp>();

        generate_obst();
        make_walls();
    }

    void on_update(float dt) {
        if (start) {
            auto now = std::chrono::steady_clock::now();
            if (create_second && std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= 1500) {
                create_second = false;
                generate_obst();
            }
            if(create_obst) {
                generate_obst();
                create_obst = false;
                score++;
            }
            engine::actions(registry, dt);
            engine::physics(registry, dt);
            if(engine::aabb_check(*this, dt)) return;
        }
        else {
            if (IsKeyPressed(KEY_ENTER)) {
                start = true;
                begin = std::chrono::steady_clock::now();
            }
        }
        engine::renderer(player, registry);
    }

    void on_end() {
    }

    bool should_close() {
        return WindowShouldClose();
    }

    void generate_obst() {
        int r = GetRandomValue(0,100);
        char obts[25];
        if (r < 30) {
            for (int i = 0; i < 25; i++)
                obts[i] = " #"[GetRandomValue(0,1)];
        }
        else {
            char* a= obsts[GetRandomValue(0,4)];
            for (int i = 0; i < 25; i++)
                obts[i] = a[i];
        }

        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                if (obts[y * 5 + -x] == ' ') continue;
                Entity tmp = create_entity();

                tmp.add_component<TransformComp>(TransformBuilder().position({(float)x, (float)y, 9.0f}).size({1.0f, 1.0f, 0.3f}));
                tmp.add_component<PrimitiveComp>(
                        PrimitiveComp::CUBE, 
                        ColorFromHSV(GetRandomValue(0, 360), 0.4f,1.0f), 
                        PRIMITVE_WIREFRAME | PRIMITVE_FILLED);
                tmp.add_component<PhysicsBodyComp>(PhysicsBodyBuilder().is_solid(false).intersects_callback(intersects_callback).build());
                tmp.add_component<ActionsComp>(ActionsComp().add(new ObstAction(&create_obst, &score, -3.0f - (0.07 * (float)(score  + 1)))));
            }
        }

    }

    void make_walls() {
        TransformComp positions[] = {
            TransformBuilder().position({2.0f, -1.0f,0.0f}).size({5.0f, 1.0f, 20.0f}).build(),
            TransformBuilder().position({2.0f,  5.0f,0.0f}).size({5.0f, 1.0f, 20.0f}).build(),
            TransformBuilder().position({-1.0f, 2.0f,0.0f}).size({1.0f, 5.0f, 20.0f}).build(),
            TransformBuilder().position({5.0f,  2.0f,0.0f}).size({1.0f, 5.0f, 20.0f}).build(),
            TransformBuilder().position({2.0f,  2.0f,10.0f}).size({5.0f, 5.0f, 1.0f}).build(),
        };

        for (int i = 0; i < 5; i++) {
            Entity tmp = create_entity();
            tmp.add_component<TransformComp>(positions[i]);
            tmp.add_component<PrimitiveComp>(
                    PrimitiveComp::CUBE, 
                    (i % 2 == 0) ?  DARKGRAY : GRAY,
                    PRIMITVE_WIREFRAME | PRIMITVE_FILLED);
            tmp.add_component<PhysicsBodyComp>(PhysicsBodyBuilder().is_static(true).is_solid(true).build());
        }
    }

private:
    Entity player;
    bool create_obst = false, create_second = true;
    std::chrono::steady_clock::time_point begin;
    int score = 0;
    bool start = false;
};


int intersects_callback(Scene& scene,entt::entity self, entt::entity other) {
    if (!scene.registry.any_of<TagComp>(other)) return 0;
    const TagComp& tag = scene.registry.get<TagComp>(other);
    if (tag.tag == "Player") {
        scene.manager->set_current("game");
        return 1;
    }
    return 0;
}
