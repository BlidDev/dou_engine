#include <epch.h>
#include "light.h"
#include "actions.h"
#include "thing.h"

void register_actions();

int engine::on_start(engine::SceneManager* manager) {

    register_actions();
    manager->main_window = Window("notray", 1280, 720);
    manager->render_data.screen_w = 1280;
    manager->render_data.screen_h = 720;
    engine::set_input_window(manager->main_window);

    std::string name;
    read_project_file("res/projects/test.prj", manager);


    manager->register_model("triangle", ModelBuilder().vertices(engine::P_TRIANGLE, 9));
    manager->register_model("quad", ModelBuilder().vertices(engine::P_QUAD, 12).indices(engine::I_QUAD, 6));
    manager->register_model("cube_tex", ModelBuilder().vertices(engine::P_CUBE_TEXTURE, 180).textured());


    manager->register_scene<LightScene>("light");
    manager->register_scene<ThingScene>("thing");
    manager->set_current("thing");
    return 0;
}


int engine::on_end(engine::SceneManager* manager) {
    glfwTerminate();
    return 0;
}

std::unique_ptr<Scene> engine::create_runtime_scene() {
    return std::make_unique<DefaultRT>();
}

void register_actions() {
    engine::ActionsComp::register_action("SimpleAct", new SimpleAct);
}
