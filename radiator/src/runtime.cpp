#include "runtime.h"

#include "editors.h"

void run_rt_scene(EScene *editor) {
    DU_INFO("Playing now");
    
    editor->save_project();

    const ProjectData& project_data = editor->manager->project_data;
    std::string path = std::format("{}/{}.prj", project_data.root_path.c_str(), project_data.name);

    SceneManager rt_manager;
    read_project_file(path.c_str(), &rt_manager, false, true);

    {
        SceneManager* emanager = editor->manager;

        rt_manager.main_window = emanager->main_window;
        rt_manager.render_data = emanager->render_data;
        ubos_shaders_bind(rt_manager.render_data, rt_manager.shader_lib);

    }
    DU_ASSERT(!rt_manager.main_window, "Runtime SceneManager main_window is null");

    glViewport(0,0, rt_manager.render_data.screen_w, rt_manager.render_data.screen_h);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    Scene* current = rt_manager.get_current();

    double start = glfwGetTime();
    while (!current->should_close())
    {
        double now = glfwGetTime();
        double dt = now - start;
        if (rt_manager.old) {
            rt_manager.end_scene(rt_manager.old);
            rt_manager.old = nullptr;
        }
        if (rt_manager.switched) { 
            current = rt_manager.get_current();
            rt_manager.switched = false;
            current->on_create();
            update_render_data(&rt_manager, current);
        }
        current->on_update(dt);
        start = now;
    }

    current->on_end();

    //editor->init_imgui();
    //editor->make_viewer();
}
