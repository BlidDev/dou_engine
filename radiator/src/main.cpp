#include <espch.h>
#include "editors.h"
#include "greeter.h"

int engine::on_start(engine::SceneManager* manager) {

    manager->main_window = Window("Radiator", 1280, 720);
    manager->render_data.screen_w = 1280;
    manager->render_data.screen_h = 720;
    engine::set_input_window(manager->main_window);

    std::string name;

    Greeter* greeter = new Greeter;
    greeter->editor = new EScene;
    manager->register_scene("EDITOREditor", greeter->editor);
    manager->register_scene("EDITORGreeter", greeter);

    manager->set_current("EDITORGreeter");

    return 0;
}


Scene* engine::create_runtime_scene() {
    return new RTScene;
}
