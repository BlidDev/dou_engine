#include "editors.h"
#include "project.h"
#include "runtime.h"
#include "helper.h"

int EScene::key_query[ GLFW_KEY_LAST - 32]= {};

EScene::EScene() : Scene("Editor") { 
    save_path = "UNSET";
    close = false; 
    for (int i = 0; i < GLFW_KEY_LAST - 32; i++)
        key_query[i] = -1;

    selected = 0;
    show_project_settings = false;
    working_scene = nullptr;
    creating_scene = false;
}


void EScene::make_viewer() {
    viewer = create_entity();
    viewer.add_component<EditorViewer>();
    viewer.add_component<TransformComp>(TransformBuilder().position(glm::vec3{0.0f}));
    viewer.add_component<CameraComp>(CameraBuilder().build());
    auto& lua = viewer.add_component<LuaActionComp>(viewer.uuid()).add(this, "res/scripts/editorcam.lua");
    lua.get_last().on_init();
}

void EScene::on_create() {
    init_imgui();


    make_default_ubos(manager);

    update_render_data(manager, this);

    make_framebuffer(editorview, 684, 698) ;
    make_framebuffer(pickerview, 684, 698) ;

    make_viewer();
    debug_open = false;

    {
        RootReseter r(&manager->project_data);
        register_shader("res/shaders/picker.glsl");
    }
    picker_shader = get_shader("picker.glsl");

    if (!working_scene) {
        if (manager->num_of_scenes() <= 2) {creating_scene = true; return;}

        for (auto [n, s] : manager->get_scenes()) {
            if (n == "EDITOREditor" || n == "EDITORGreeter") continue;
            working_scene = s;

            if (working_scene->file_path.empty()) return;
            save_path = working_scene->file_path;
            working_scene->add_from_file(working_scene->file_path.c_str()); return;
            manager->project_data.startup_scene = s->name;
        }

//        working_scene = manager->register_scene("Unnamed RT", create_runtime_scene());
 //       manager->project_data.startup_scene = working_scene->name;
    }
    else {
        if(!working_scene->uuids.empty() || working_scene->file_path.empty()) return;
        working_scene->add_from_file(working_scene->file_path.c_str());
    }
}

void EScene::on_update(float dt) {
    int _debug[] = {GLFW_KEY_LEFT_SHIFT, GLFW_KEY_F9};
    if (check_key_combo(_debug, 2) && !debug_open) {
        debug_open = true;
        working_scene->add_from_file("res/scenes/editortest.scene");
        save_path = "res/scenes/editortest.scene";
    }
    if (is_key_pressed(GLFW_KEY_ESCAPE)) {
        selected = 0;
    }

    if(update_imgui(dt) == EditorState::EditorPreview) {
        run_rt_scene(this);
    }
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}

void EScene::on_end() {
    end_imgui();
}

bool EScene::should_close() {
  return glfwWindowShouldClose(manager->main_window) || close;
}


Scene* EScene::create_scene(const char* name) {
    Scene* tmp = manager->register_scene(name, create_runtime_scene());
    tmp->name = name;
    return tmp;
}
