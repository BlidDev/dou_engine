#include <epch.h>
#include "actions.h"
#include "light.h"
#include "thing.h"

void register_actions();

int engine::on_start(engine::SceneManager* manager) {

    register_actions();
    manager->main_window = Window("notray", 848, 480);
    engine::set_input_window(manager->main_window);

    manager->register_shader("res/shaders/basic.glsl");
    manager->register_shader("res/shaders/textured.glsl");
    manager->register_texture("res/textures/proto.png");

    manager->register_model("triangle", ModelBuilder().vertices(engine::P_TRIANGLE, 9));
    manager->register_model("quad", ModelBuilder().vertices(engine::P_QUAD, 12).indices(engine::I_QUAD, 6));
    manager->register_model("cube", ModelBuilder().vertices(engine::P_CUBE, 108));
    manager->register_model("cube_tex", ModelBuilder().vertices(engine::P_CUBE_TEXTURE, 180).textured());


    manager->register_scene("light", new LightScene());
    manager->register_scene("thing", new ThingScene());
    manager->set_current("thing");
    return 0;
}


int engine::on_end() {
    glfwTerminate();
    return 0;
}

void register_actions() {
    engine::ActionsComp::register_action("SimpleAct", new SimpleAct);
}
