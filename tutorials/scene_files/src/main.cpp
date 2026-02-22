// src/main.cpp

#include <engine.h>
#include "fps_ctrl.h"
#include "test_scene.h"



int engine::on_start(engine::SceneManager* manager) {
    // Create the object
    manager->main_window = Window("Test Game", 1280, 720);
    engine::set_input_window(manager->main_window);

    // Initial setup
    set_clear_flags(manager->render_data, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Mesh setup
    manager->register_mesh("triangle",engine::MeshBuilder()
                           .vertices(engine::P_TRIANGLE, engine::P_TRIANGLE_S)
                           ); // Not moved to .sff for the sake of the tutorial 

    read_project_file("res/test_project.prj", manager, true, true);
    make_default_ubos(manager);


    engine::LuaManager::init("res/util.lua");
    return 0;
}

int engine::on_end(engine::SceneManager* manager) {
    return 0;
}

std::unique_ptr<engine::Scene> engine::create_runtime_scene() {
    return std::make_unique<engine::DefaultRT>();
}

