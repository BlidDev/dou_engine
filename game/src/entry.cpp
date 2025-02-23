#include <epch.h>
#include "macros.h"
#include "components/action.h"
#include "light.h"

void register_actions();

int engine::on_start(engine::SceneManager* manager) {

    //register_actions();
    //manager->register_scene("main", new MainScene());
    //manager->register_scene("win", new WinScene());
    //manager->register_scene("game", new GameScene());
    //manager->register_scene("test", new TestScene());
    manager->register_scene("light", new LightScene());
    manager->set_current("light");

    manager->main_window = Window("notray", 848, 480);
    return 0;
}


int engine::on_end() {

    glfwTerminate();
    return 0;
}

void register_actions() {
    //engine::ActionsComp::register_action("PlayerAction", new PlayerAction);
    //engine::ActionsComp::register_action("FPSAction", new FPSAction(0));
    //engine::ActionsComp::register_action("CubeAction", new CubeAction);
}
