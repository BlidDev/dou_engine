#include "editors.h"
#include "runtime.h"

int EScene::key_query[ GLFW_KEY_LAST - 32]= {};

EScene::EScene() : Scene("Editor") { 
    save_path = "UNSET";
    close = false; 
    for (int i = 0; i < GLFW_KEY_LAST - 32; i++)
        key_query[i] = -1;

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

    glfwSetKeyCallback(manager->main_window, key_callback);
    make_default_ubos(manager);

    update_render_data(manager, this);

    make_framebuffer(editorview, 684, 698) ;

    make_viewer();
}

void EScene::on_update(float dt) {
    //close = is_key_pressed(GLFW_KEY_ESCAPE);


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


