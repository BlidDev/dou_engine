// src/main.cpp

#include <engine.h>
#include "test_scene.h"



int engine::on_start(engine::SceneManager* manager) {
    // Create the object
    manager->main_window = Window("Test Game", 1280, 720);
    engine::set_input_window(manager->main_window);

    // Initial setup
    set_clear_flags(manager->render_data, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set_layer_depth(manager->render_data, 0, true);

    // Shader setup
    manager->register_shader("res/shaders/basic.glsl");
    manager->register_shader("res/shaders/camera_present.glsl");
    make_default_ubos(manager);


    // Mesh setup
    manager->register_mesh("triangle",engine::MeshBuilder()
                           .vertices(engine::P_TRIANGLE, engine::P_TRIANGLE_S)
                           );
    manager->register_mesh("quad_tex", mesh_from_file("res/meshes/quad_tex.sff"));
    manager->register_mesh("cube", mesh_from_file("res/meshes/cube.sff"));

    // Script setup
    manager->register_script("res/scripts/fps_controller.lua"); 
    manager->register_script("res/scripts/trigger.lua"); 

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

