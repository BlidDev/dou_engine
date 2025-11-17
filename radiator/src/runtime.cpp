#include "runtime.h"

#include "editors.h"

void run_rt_scene(EScene *editor) {
    DU_INFO("Playing now");
    
    auto manager = editor->manager;
    save_working_file(manager, editor);

    manager->current = "working";
    manager->switched = true;

    glViewport(0,0, manager->render_data.screen_w, manager->render_data.screen_h);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Scene* current = editor->working_scene;

    double start = glfwGetTime();
    while (!current->should_close())
    {
        double now = glfwGetTime();
        double dt = now - start;
        if (manager->switched) { 
            current = manager->get_current();
            manager->switched = false;
            current->on_create();
            update_render_data(manager, current);
        }
        current->on_update(dt);
        start = now;
    }

    manager->end_scene(editor->working_scene);
    editor->working_scene->add_from_file(editor->save_path.c_str());


    manager->current = "Editor";
    manager->switched = false;
    //editor->init_imgui();
    //editor->make_viewer();
}
