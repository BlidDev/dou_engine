#include <espch.h>
#include "editors.h"

int engine::on_start(engine::SceneManager* manager) {

    manager->main_window = Window("Radiator", 1280, 720);
    manager->render_data.screen_w = 1280;
    manager->render_data.screen_h = 720;
    engine::set_input_window(manager->main_window);

    std::string name;

    EScene* editor = new EScene();
    editor->working_scene = new RTScene();
    manager->register_scene("working", editor->working_scene);
    manager->register_scene("Editor", editor);

    EG_ASSERT(!editor->working_scene, "Working scene not set");
    read_project_file("res/projects/editor.prj", manager, &name, true);

    return 0;
}
