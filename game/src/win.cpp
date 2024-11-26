#include "win.hpp"

using namespace engine;

WinScene::WinScene() : Scene("win") {
}

void WinScene::on_create() {
    text = registry.create();
    registry.emplace<TextComp>(text, "YOU WIN!! :D", 40,BLACK);
    registry.emplace<TransformComp>(text, (Vector3){370.0f,2.0f,0.0f});
    EnableCursor();
}

void WinScene::on_update(float dt) {
    int key = GetKeyPressed();
    if (key != 0 && key != KEY_F) {
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

bool WinScene::should_close() {
    return WindowShouldClose();
}

void WinScene::on_end() {
    EG_TRACE("win end called\n");
}

