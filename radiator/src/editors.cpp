#include "editors.h"
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

    picker_shader = get_shader("res/shaders/picker.glsl");
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

    if(update_imgui(dt) == EditorState::Preview) {
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


