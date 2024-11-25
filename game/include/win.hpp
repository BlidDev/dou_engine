#pragma once
#include <epch.h>


using namespace engine;

class WinScene : public engine::Scene {
public:
    WinScene() : Scene("win") {
    }

    void on_create() {
        text = registry.create();
        registry.emplace<TextComp>(text, "YOU WIN!! :D", 40,BLACK);
        registry.emplace<TransformComp>(text, (Vector3){370.0f,2.0f,0.0f});
        EnableCursor();
    }

    void on_update(float dt) {
        if (GetKeyPressed() != 0) {
            manager->set_current("test");
            return;
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);

        auto& tex = registry.get<TextComp>(text);
        auto& pos = registry.get<TransformComp>(text);

        DrawText(tex.body.c_str(), pos.position.x, pos.position.y,tex.font_size, tex.color);
        EndDrawing();
    }

    bool should_close() {
        return WindowShouldClose();
    }

    void on_end() {
        EG_TRACE("win end called\n");
    }
    
    entt::entity text;

};
