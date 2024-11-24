#include <raylib.h>
#include <engine.h>

#include "components/action.h"
#include "main.hpp"
#include "game.hpp"
#include "win.hpp"
#include "test.hpp"

void register_actions();

int engine::on_start(engine::SceneManager* manager) {

    register_actions();
    manager->register_scene("main", new MainScene());
    manager->register_scene("win", new WinScene());
    manager->register_scene("game", new GameScene());
    manager->register_scene("test", new TestScene());
    manager->set_current("main");



    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(848, 480, "raygame");
    return 0;
}


int engine::on_end() {

    CloseWindow();
    return 0;
}

void register_actions() {
    ActionsComp::register_action("PlayerAction", new PlayerAction);
    ActionsComp::register_action("FPSAction", new FPSAction(0));
    ActionsComp::register_action("CubeAction", new CubeAction);

}
