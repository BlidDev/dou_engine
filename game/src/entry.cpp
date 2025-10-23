#include <epch.h>
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
    read_project_file("res/projects/test.prj", manager, &name);

    manager->render_data.add("Matrices", 2 * sizeof(glm::mat4));
    manager->render_data.add("Lighting", 2 * sizeof(glm::vec4));

    size_t max = 32;
    size_t dir_size = sizeof(engine::DirLightComp); 
    size_t pnt_size = 3 * sizeof(glm::vec4); 
    size_t spt_size = sizeof(SptLightComp) + sizeof(glm::vec4);

    manager->render_data.add("SptLights", spt_size * max + sizeof(int));
    manager->render_data.add("DirLights", dir_size * max + sizeof(int));
    manager->render_data.add("PntLights", pnt_size * max + sizeof(int));

    engine::ubos_shaders_bind(manager->render_data, manager->shader_lib);

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
