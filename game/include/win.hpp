#pragma once
#include <engine.h>
#include <raylib.h>
#include <stdio.h>


class WinScene : public engine::Scene {
public:
    WinScene() : Scene() {
    }

    void on_create() {
        text = registry.create();
        registry.emplace<engine::Text>(text, "YOU WIN!! :D", 40,BLACK);
        registry.emplace<engine::Transform>(text, (Vector3){370.0f,2.0f,0.0f});
        EnableCursor();
    }

    void on_update(float dt) {
        if (GetKeyPressed() != 0) {
            manager->set_current("main");
            return;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        auto& tex = registry.get<engine::Text>(text);
        auto& pos = registry.get<engine::Transform>(text);

        DrawText(tex.body.c_str(), pos.position.x, pos.position.y,tex.font_size, tex.color);
        EndDrawing();
    }

    bool should_close() {
        return WindowShouldClose();
    }

    void on_end() {
        printf("win end called\n");
    }
    
    entt::entity text;

};
