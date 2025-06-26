#include <epch.h>
#include "actions.h"
#include "light.h"
#include "model.h"
#include "thing.h"

void register_actions();

int engine::on_start(engine::SceneManager* manager) {

    register_actions();
    manager->main_window = Window("notray", 848, 480);
    engine::set_input_window(manager->main_window);

    manager->register_shader("res/shaders/basic.glsl");
    manager->register_shader("res/shaders/textured.glsl");
    manager->register_shader("res/shaders/lightingt.glsl");
    manager->register_shader("res/shaders/lighting.glsl");
    manager->register_texture("res/textures/proto.png");

    manager->render_data.add("Matrices", 2 * sizeof(glm::mat4));
    manager->render_data.add("Lighting", 2 * sizeof(glm::vec4));

    size_t max = 32;
    size_t dir_size = sizeof(engine::DirLightComp);   // MUST be 32
    size_t pnt_size = sizeof(engine::PntLightComp);   // MUST be 32
    size_t pos_size = sizeof(glm::vec4);   // MUST be 32

    size_t array_data_size = max * (dir_size + pnt_size + pos_size); // 1024 + 1024 = 2048

    // The two floats with padding (16 bytes for std140 alignment)
    size_t floats_size = 16;
    size_t total_size = array_data_size + floats_size;
    manager->render_data.add("SceneLights", total_size);

    engine::ubos_shaders_bind(manager->render_data, manager->shader_lib);

    manager->register_model("quad_tex", model_from_file("res/models/quad_tex.sff"));
    manager->register_model("triangle", ModelBuilder().vertices(engine::P_TRIANGLE, 9));
    manager->register_model("quad", ModelBuilder().vertices(engine::P_QUAD, 12).indices(engine::I_QUAD, 6));
    manager->register_model("cube", ModelBuilder().vertices(engine::P_CUBE, 108));
    manager->register_model("cube_tex", ModelBuilder().vertices(engine::P_CUBE_TEXTURE, 180).textured());
    manager->register_model("cube_tl", model_from_file("res/models/cube_tl.sff"));


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
