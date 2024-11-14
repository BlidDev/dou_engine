#include <raylib.h>
#include <engine.h>

#include "main.hpp"
#include "game.hpp"
#include "win.hpp"

int engine::on_start(engine::SceneManager* manager) {

    manager->register_scene("main", new MainScene());
    manager->register_scene("win", new WinScene());
    manager->register_scene("game", new GameScene());
    manager->set_current("main");


    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(848, 480, "raygame");
    return 0;
}


int engine::on_end() {

    CloseWindow();
    return 0;
}
