#include "RTscene.h"

RTScene::RTScene() : Scene("RT") { close = false; }


void RTScene::on_create() {
    lua_action_init(this);
}

void RTScene::on_update(float dt) {
    close = is_key_pressed(GLFW_KEY_ESCAPE);


    
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}

void RTScene::on_end() {

  lua_action_end(this);
}

bool RTScene::should_close() {
  return glfwWindowShouldClose(manager->main_window) || close;
}


