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

    // Shader setup
    manager->register_shader("res/basic.glsl");
    manager->register_shader("res/camera_present.glsl");
    make_default_ubos(manager);


    // Mesh setup
    manager->register_mesh("triangle",engine::MeshBuilder()
                           .vertices(engine::P_TRIANGLE, engine::P_TRIANGLE_S)
                           );
    manager->register_mesh("quad_tex", mesh_from_file("res/quad_tex.sff"));
    manager->register_mesh("cube", mesh_from_file("res/cube.sff"));

    // Script setup
    manager->register_script("res/fps_controller.lua"); 
    manager->register_script("res/trigger.lua"); 

    manager->register_scene<TestScene>("Test");
    manager->set_current("Test");
    return 0;
}

int engine::on_end(engine::SceneManager* manager) {
    return 0;
}

std::unique_ptr<engine::Scene> engine::create_runtime_scene() {
    return std::make_unique<engine::DefaultRT>();
}

