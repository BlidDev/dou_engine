#include <epch.h>
#include "bscene.h"

int engine::on_start(engine::SceneManager* manager) {

    LuaManager::init("res/util.lua");

    unsigned int factor = 50;
    unsigned int ww = 16 * factor, wh = 9 * factor;
    manager->main_window = Window("notray", ww, wh);
    manager->render_data.screen_w = ww;
    manager->render_data.screen_h = wh;


    set_input_window(manager->main_window);
    set_clear_flags(manager->render_data, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    make_default_ubos(manager);



    manager->register_mesh("triangle", MeshBuilder().vertices(engine::P_TRIANGLE, 9));
    manager->register_mesh("quad", MeshBuilder().vertices(engine::P_QUAD, 12).indices(engine::I_QUAD, 6));


    read_project_file("res/projects/test.prj", manager);
    ubos_shaders_bind(manager->render_data, manager->shader_lib);

    manager->register_scene<BScene>("bscene");


    manager->set_current("bscene");
    return 0;
}


int engine::on_end(engine::SceneManager* manager) {
    return 0;
}

std::unique_ptr<engine::Scene> engine::create_runtime_scene() {
    return std::make_unique<engine::DefaultRT>();
}

