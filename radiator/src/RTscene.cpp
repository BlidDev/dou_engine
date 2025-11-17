#include "RTscene.h"

RTScene::RTScene() : Scene("RT") { 
    close = false; 
}


void RTScene::on_create() {
    player = uuid_to_entity(main_camera);
    lua_action_init(this);
}

void RTScene::on_update(float dt) {
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    close = is_key_pressed(GLFW_KEY_ESCAPE);
    actions(this,dt);
    lua_action_update(this, dt);
    physics(registry,dt);
    if (aabb_check(*this, dt)) return;
    glm::vec2 view = manager->main_window.size();

    opengl_renderer(manager->render_data,view, player, registry);
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();

}

void RTScene::on_end() {

  lua_action_end(this);
  close = false;
}

bool RTScene::should_close() {
  return glfwWindowShouldClose(manager->main_window) || close;
}


