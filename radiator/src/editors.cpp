#include "editors.h"

EScene::EScene() : Scene("Editor") { close = false; }


void EScene::on_create() {
    init_imgui();
    lua_action_init(this);

    add_from_file("res/scenes/editortest.scene");
    update_render_data(manager, this);
}

void EScene::on_update(float dt) {
    //close = is_key_pressed(GLFW_KEY_ESCAPE);

    update_imgui();
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}

void EScene::on_end() {
    end_imgui();
    lua_action_end(this);
}

bool EScene::should_close() {
  return glfwWindowShouldClose(manager->main_window) || close;
}


